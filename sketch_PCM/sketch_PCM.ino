/*
  Modbus RTU Server Four Channel Pump Control Module
  Version 0.1
  Features
    ToDo: X - finished * - to be debugged  
      * - Active configuration. Modbus Address and protocol settings
        - Speed control with individual speed calibration
        - Stepper Motor Support
        - Servo Motor Support
      * - enable debug direct control
      * - replace dummy RunDCMotor function
        - write ISR in assembly for max speed.
      * - pin selection - for Arduino Micro
      * - pin selection - for Arduino Uno R3
      * - MCU Porting to ATmega328p
        - Need ideas to alter the number of motors accessed by modbus
      General cleanup and optimization.

  Bugs
       
  Circuit:
   - Arduino Nano v3.0 or
   - Arduino Micro
   - (2) L298N Dev board
   - MAX485 Dev board
   - 10k >= POT
   - DC motor 7-32V
   - Stepper Motor (bipolar)
   - Servo Motor (5v logic)
  created 2023-11-9 
  by R.Rainwater
*/
#include "Arduino.h"
#include "pins.h" 
#include "use_modbus.h"
#include "feedback.h"
#include "my_useful_functions.h"
#include "globals.h"
#include "src/motors/_motor_drivers.h"

//#define DEBUG_MY_PROGRAM
#ifdef DEBUG_MY_PROGRAM
  unsigned int debug_test_value = 0;
  bool debug_state_change = true;
  #define DEBUG_STATE_CHANGE debug_state_change = true;
  void HandleSerialInput(); // !!!only works on the MICRO, will break modbus && serial on the Uno!!!!!
#else
  #define DEBUG_STATE_CHANGE
#endif

int freeRam(){ extern int __heap_start,*__brkval; unsigned int a;return(int)&a-(__brkval==0?(int)&__heap_start:(int) __brkval);}
/////end of debug stuff

void RunStateMachine(unsigned char c, unsigned char d, unsigned char h, unsigned char i, unsigned char pump_number);

void setup() {

  // configure modbus address via serial and potentiometer
  // if no change is detected. no writes to the com line will be made
  // Do not adjust potentiometer while connected to Modbus line, this will violate the protocol!!!!!!!!
  // has no effect after startup.
  // Give the user 5 seconds to make first adjustment, after that lock out this feature.
  int address;
  {
    int previous_value;
    #ifdef DEBUG_MY_PROGRAM
      pinMode(POT_IN, INPUT_PULLUP);
    #else
      pinMode(POT_IN, INPUT);
    #endif
    analogRead(POT_IN);
    delay(100);
    //BugFix:cant set address to 1 or 32 bug. Pad the analogread() by 5% on the ends.
    address = map( constrain(analogRead(POT_IN), 50, 972) , 50, 972, 1, 32); // Get address from the pot
    previous_value = address;
    Serial.begin(9600);
    unsigned long time = millis()+5000;
    while( millis() < time ){
      address  = map( constrain(analogRead(POT_IN), 50, 972) , 50, 972, 1, 32);
      if(address != previous_value){
        time = millis() + 10000; //give user another 10 seconds to change the address
        Serial.println( address );
        previous_value = address;
      }
    }
    #ifndef DEBUG_MY_PROGRAM 
      Serial.end();
    #endif
  }
  setup_pins();
  setup_modbus(address);
  setup_feedback();
  DEBUG_STATE_CHANGE
}

unsigned long LoopClock=0;
unsigned long loops_per_second = 0;

void loop() {  
  _modbus.poll();
  //Report how fast this loop is excuiting.
  loops_per_second++;
  if( millis() - LoopClock  >= 1000 ){
    LoopClock = millis();
    ir_data[IR_LOOP_SPEED] = loops_per_second;
    ir_data[IR_FREE_RAM]   = freeRam();
    loops_per_second = 0;
  }
  RunStateMachine(PUMP_1_C, PUMP_1_D, PUMP_1_H, PUMP_1_I, 0);
  RunStateMachine(PUMP_2_C, PUMP_2_D, PUMP_2_H, PUMP_2_I, 1);
  RunStateMachine(PUMP_3_C, PUMP_3_D, PUMP_3_H, PUMP_3_I, 2);
  RunStateMachine(PUMP_4_C, PUMP_4_D, PUMP_4_H, PUMP_4_I, 3);

  #ifdef DEBUG_MY_PROGRAM
    HandleSerialInput();
  #endif
}

void RunStateMachine(unsigned char c, unsigned char d, unsigned char h, unsigned char i, unsigned char pump_number){
  uint16_t *pump = pump_values[pump_number];//Pointer to current pump. makes code more readable

  //record the enable coil value
  SETBIT(pump[PUMP_STATE], ENABLE_BIT, coil_data[ c + COIL_ENABLE ]);

  //update Modbus total dispenced units 
  if(pump[PUMP_CAL])
    ir_data[ i + IR_TOTAL] = pump_counters[pump_number]/pump[PUMP_CAL];

  //run current state of the machine
  switch(STATE_BITS & pump[PUMP_STATE]){
    case 0x00: //idle state

      //exit if enable bit is low
      if( !READBIT(pump[PUMP_STATE], ENABLE_BIT) ) 
        break;
      //Set next State
      pump[PUMP_STATE] += 1;
      DEBUG_STATE_CHANGE

      //store operational paramaters.
      SETBIT(pump[PUMP_STATE], ENABLE_BIT, 1);
      SETBIT(pump[PUMP_STATE], FEEDBACK_BIT, coil_data[c + COIL_FEEDBACK ] );
      SETBIT(pump[PUMP_STATE], DEBUG_BIT,    coil_data[c + COIL_DEBUG ] );
      SETBIT(pump[PUMP_STATE], INVERT_BIT,   coil_data[c + COIL_INVERT ] );
      pump[PUMP_CAL]      = hr_data[ h + HR_CAL ];
      pump[PUMP_DURATION] = hr_data[ h + HR_DURATION ];
      pump[PUMP_DOSE]     = hr_data[ h + HR_DOSE ];
      pump[PUMP_STATE]   |= hr_data[ h + HR_MOTOR_TYPE ] << 2;
      // value verification
      if( pump[PUMP_CAL] == 0     ){ pump[PUMP_CAL]      = 1; }
      if( pump[PUMP_DURATION] == 0){ pump[PUMP_DURATION] = 1; }
      
      //configure feedback counter
      pump_counters[pump_number] = 0; //reset counter
      if( READBIT( pump[PUMP_STATE], FEEDBACK_BIT ) ){
        enable_feedback(pump_number);
      }else{
        disable_feedback(pump_number);  
      }

      {
        float total, cycles, dose;
        total = (float)pump[PUMP_CAL] * (float)pump[PUMP_DOSE]; //TOTAL_FEEDBACK needed to satisfy request = CAL * DOSE;        
        cycles = (float)pump[PUMP_DURATION];             //Total one second CYCLES
        dose = total / cycles;                    //Total amount to DOSE per cycle = total/cycles

        if( total >= 65536 ) {         //ERROR can not pump more than we can count.
          pump[PUMP_STATE] |= 0x03;    //enter last state
          di_data[ i + DI_ERROR ] = 1; //set error flag
          break;
        }
        if( dose == 0){ dose = 1; } //fix possible of a 0 dose. 
        //store values
        ir_data[ i + IR_TOTAL_FEEDBACK ] = total;
        ir_data[ i + IR_CYCLES ]         = cycles;
        ir_data[ i + IR_DOSE ]           =  dose;
      }
      //initalize some data
      ir_data[ i + IR_TIMER_VALUE ] = 0;  //set cycle timer to 0
      ir_data[ i + IR_TOTAL ] = 0;        //set total dispenced to 0
      di_data[ d + DI_BUSY ]  = 1;        //set busy flag to 1 
      di_data[ d + DI_ERROR ] = 0;        //clear error flag
      setup_motor(pump_number);
    break;

    case 0x01: //start a pumping cycle

      //exit if its too soon to start another cycle.
      if( millis() - ir_data[ i + IR_TIMER_VALUE ] <= 1000 )
        break;
      //GoTo last stage if enable is off
      //or the request has been saticfied and debug is false
      if( !READBIT(pump[PUMP_STATE], ENABLE_BIT)
      ||
        ( ir_data[ i + IR_TOTAL_FEEDBACK ] <= pump_counters[pump_number]
        &&
        !READBIT( pump[PUMP_STATE], DEBUG_BIT) )
      ){
        pump[PUMP_STATE] |= 0x03;
        di_data[ d + DI_BUSY ] = 0;
        DEBUG_STATE_CHANGE
        break;
      }

      //save the starting point to determin the stopping point
      pump[PUMP_CYCLE_START_FEEDBACK] = pump_counters[pump_number];  
      //save the time the cycle started.
      ir_data[ i + IR_TIMER_VALUE ] = millis();
      //update modbus cycle counter register
      ir_data[ IR_CYCLES ] -= 1;
      //run the motor
      run_motor(pump_number, 255);
      //Set next State
      pump[PUMP_STATE] += 1;
      DEBUG_STATE_CHANGE
    break;

    case 0x02: //Stop a pump cycle
      {
        // check if the request has been saticfied 
        // or the dose has been reached 
        // or the enable bit is low
        if( ir_data[ i + IR_TOTAL_FEEDBACK ] <= pump_counters[pump_number] 
        ||
            (pump[PUMP_DURATION]-ir_data[IR_CYCLES]) * ir_data[ i + IR_DOSE ] <= pump_counters[pump_number]
        ||
            !READBIT(pump[PUMP_STATE], ENABLE_BIT)
        ){
          if( READBIT( pump[PUMP_STATE], DEBUG_BIT) && READBIT(pump[PUMP_STATE], ENABLE_BIT))
            break;
          //Stop the pump
          run_motor(pump_number, 0);
          pump[PUMP_STATE] -=1; //GoTo previous stage
          DEBUG_STATE_CHANGE
          break;
        }
        work_motor(pump_number);
      }
    break;

    default: //Final stage, lock out the state machine until ENABLE_BIT goes low.
      //Check for reset condition
      if( !READBIT(pump[PUMP_STATE], ENABLE_BIT) ){
        di_data[ d + DI_ERROR] = 0; //Clear error flag on reset
        pump[PUMP_STATE] &= ~0x03;
        stop_motor(pump_number);
        DEBUG_STATE_CHANGE
        break;
      }
  }// End of switch statement
  return;
}

#ifdef DEBUG_MY_PROGRAM
  void HandleSerialInput(){
    uint16_t *pump = pump_values[0];
    /*
    if( debug_state_change && ( (STATE_BITS & pump[PUMP_STATE]) != 0x02) ){
      debug_state_change = false;

      Serial.println(F(""));
      
      switch( pump[0] & STATE_BITS ){
        case 0: Serial.println(F("idle")); break;
        case 1: Serial.println(F("start")); break;
        case 2: Serial.println(F("stop")); break;
        case 4: Serial.println(F("done")); break;
      }
      Serial.print(F("e:"));            Serial.println( READBIT( pump[PUMP_STATE], ENABLE_BIT));
      Serial.print(F("f:"));            Serial.println( READBIT( pump[PUMP_STATE], FEEDBACK_BIT));
      Serial.print(F("d:"));            Serial.println( READBIT( pump[PUMP_STATE], DEBUG_BIT));
      Serial.print(F("i:"));            Serial.println( READBIT( pump[PUMP_STATE], INVERT_BIT));
      Serial.print(F("b:"));            Serial.println( di_data[ DI_BUSY ] );
      Serial.print(F("e:"));            Serial.println( di_data[ DI_ERROR ] );
      Serial.flush();
      Serial.print(F("Cal:"));          Serial.println( pump[PUMP_CAL]);
      Serial.print(F("Duration:"));     Serial.println( pump[PUMP_DURATION]);
      Serial.print(F("Total:"));        Serial.println( pump[PUMP_DOSE]);
      
      Serial.print(F("Motor:"));        Serial.println( pump[HR_MOTOR_TYPE]);
      
      Serial.print(F("Cycle Start:"));  Serial.println( pump[PUMP_CYCLE_START_FEEDBACK]);
      Serial.flush();
      Serial.print(F("Total request:")); Serial.println( ir_data[IR_TOTAL_FEEDBACK] );
      Serial.print(F("current feedback:"));Serial.println( pump_counters[0] );
      Serial.print(F("Cycles:"));       Serial.println( ir_data[ IR_CYCLES ] );
      Serial.print(F("Dose:"));         Serial.println( ir_data[ IR_DOSE ] );
      //Serial.flush();

      Serial.print(F("Timer:"));        Serial.println( ir_data[ IR_TIMER_VALUE ] );
      Serial.print(F("total units:"));   Serial.println( ir_data[ IR_TOTAL ] );
      
      Serial.print(F("Ram:"));          Serial.println( freeRam() );
      Serial.print(F("Loops:"));        Serial.println( ir_data[ IR_LOOP_SPEED ] );
      
      Serial.println("");
      //Serial.flush();
      
    }
*/
    while(Serial.available()){
      if( !di_data[ DI_BUSY ] )
        debug_test_value += 1;
      while(Serial.available()) {Serial.read();}  //clear read buffer
    }
    
    if( debug_test_value ){
      // Inject command into the state machine
      switch(debug_test_value){
        case 1:
          coil_data[ COIL_ENABLE ] = 1;
          coil_data[ COIL_FEEDBACK ] = 0;
          hr_data[ HR_CAL ] = 10000;
          hr_data[ HR_DURATION ] = 1;
          hr_data[ HR_DOSE ] = 6;
          hr_data[ HR_MOTOR_TYPE ] = MOTOR_TYPE_PULSE_STEP;
          debug_test_value += 1;
        break;

        case 3: //Clear command and reset the 
          coil_data[ COIL_ENABLE ] = 0;
          debug_test_value = 0;
        break;

      }//end of switch
    }
  }
#endif
