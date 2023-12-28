/*
  Modbus RTU Server Four Channel Pump Control Module
  Version 0.1
  Features
    ToDo: X - finished * - to be debugged  
      * - Active configuration. Modbus Address and protocol settings
        - Speed control with individual speed calibration
      X - Stepper Motor Support
      X - Servo Motor Support
      * - enable debug direct control
      X - replace dummy RunDCMotor function
        - write ISR in assembly for max speed.
      X - pin selection - for Arduino Micro
      X - pin selection - for Arduino Uno R3
      X - MCU Porting to ATmega328p        
      X - General cleanup and optimization.

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
#include "cPCM.h"

//Supported Motors
#include "src/motors/DCMotor.h"
#include "src/motors/Servo_motor.h"
#include "src/motors/Stepper_motor.h"
#include "src/motors/PulseStep_motor.h"

#define BLINK_ON_STARTUP 100
//#define DEBUG_FORCE_ADDRESS 1

int freeRam(){ extern int __heap_start,*__brkval; unsigned int a;return(int)&a-(__brkval==0?(int)&__heap_start:(int) __brkval);}
/////end of debug stuff
cPCM pump[4];

void setup() {
  #ifdef BLINK_ON_STARTUP
    pinMode(13, OUTPUT);
    for( int i = 10; i; i--){
      digitalWrite(13, i%2);
      delay(BLINK_ON_STARTUP);
    }
    digitalWrite(13, LOW);
  #endif

  setup_pins();
  // configure modbus address via serial and potentiometer
  // if no change is detected. no writes to the com line will be made
  // Do not adjust potentiometer while connected to Modbus line, this will violate the protocol!!!!!!!!
  // has no effect after startup.
  // Give the user 5 seconds to make first adjustment, after that lock out this feature.
  int address;  
  {
    #if defined(DEBUG_MY_PROGRAM) || defined(DEBUG_FORCE_ADDRESS)
      pinMode(POT_IN, INPUT_PULLUP);
    #else
      pinMode(POT_IN, INPUT);
    #endif
    int previous_value;
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
  #ifdef DEBUG_FORCE_ADDRESS
    address = DEBUG_FORCE_ADDRESS;
  #endif

  //startup the modbus library
  setup_modbus(address);

  //sutup the mcu pin change intrupt service
  setup_feedback();

  d_println( "setup cPCM" )
  //configure the indivisual pump control units
  {
    cPCM* p = &pump[0]; 
    for(uint8_t i = 0; i != MAX_NUM_PUMP; i++, p++){
      pump_counters[i] = 0;
      p->c = &coil_data[coil_total*i];
      p->d = &di_data[descrete_input_total*i];
      p->h = &hr_data[hold_register_total*i];
      p->i = &ir_data[input_register_total*i];
      p->feedback_counter = &pump_counters[i];
      p->id = i;
    }
  }
  d_println( "zero modbus" )
  //zero out all the modbus data
  for(uint16_t i = 0; i < coil_total*MAX_NUM_PUMP; i++)
    coil_data[i] = 0;
  for(uint16_t i = 0; i < descrete_input_total *MAX_NUM_PUMP; i++)
    di_data[i] = 0;
  for(uint16_t i = 0; i < hold_register_total*MAX_NUM_PUMP; i++)
    hr_data[i] = 0;
  for(uint16_t i = 0; i < input_register_total*MAX_NUM_PUMP+special_input_register_size; i++)
    ir_data[i] = 0;
}

uint32_t LoopClock         = 0;
uint32_t loops_per_second  = 0;
uint32_t clock             = 0;

//bool debug_trigger = true;

void loop() { 
  _modbus.poll();
/*if(debug_trigger && millis() > 20000){
  d_println("Debug Triggered")
  debug_trigger = false;
  coil_data[0] = 1;
  hr_data[0] = 1000;
  hr_data[1] = 100;
  hr_data[2] = 100;
}*/

#ifndef DEBUG_MY_PROGRAM
  for(uint8_t i = 0; i != MAX_NUM_PUMP; i++)
    pump[i].run();
#else
  pump[0].run();
#endif

  // Report UpTime
  if( millis() - clock >= 1000 ){
    clock = millis();
    ir_data[input_register_total * MAX_NUM_PUMP + IR_CLOCK] = millis()/1000;
  }
  
  //Report how fast this loop is excuiting.
  loops_per_second++;
  if( millis() - LoopClock  >= 10000 ){
    d_print("report")
    LoopClock = millis();
    ir_data[input_register_total * MAX_NUM_PUMP + IR_FREE_RAM]    = freeRam();
    ir_data[input_register_total * MAX_NUM_PUMP + IR_LOOP_SPEED]  = loops_per_second / 10;
    loops_per_second = 0;
    d_println(" finished")
  }
}

//Motor type factory.
cMotor_Base_Class* cPCM::GetNewMotor(uint8_t id){
  uint8_t type = hr_data[id * hold_register_total + HR_MOTOR_TYPE];
  switch(type){
    case MOTOR_TYPE_DC:
      return new cDCMotor;
    case MOTOR_TYPE_SERVO:
      return new cServoMotor;
    case MOTOR_TYPE_STEPPER:
      return new cStepperMotor;
    case MOTOR_TYPE_PULSE_STEP:
      return new cPulseStep;
  }
  return 0;
}
