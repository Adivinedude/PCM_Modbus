#include "cPCM.h"
#include "Arduino.h"
#include "my_useful_functions.h"
#include "register_mapping.h"
#include "feedback.h"
#include "src/motors/motor_driver.h"

cPCM::cPCM(){
    pump_state          = 0;
    calibration         = 0;
    duration            = 0;
    dose                = 0;
    start_feedback      = 0;
    total_feedback      = 0;
    timer               = 0;
    motor               = 0;
}
void cPCM::run(){
  //record the enable coil value
  SETBIT(pump_state, ENABLE_BIT, c[COIL_ENABLE]);
  //update Modbus total dispenced units 
  if(calibration)
    i[IR_TOTAL] = (uint32_t)(*feedback_counter)/calibration;
  //update Modbus feedback counter
    i[IR_FEEDBACK_COUNTER_HIGH] = (uint32_t)(*feedback_counter) >> 16;
    i[IR_FEEDBACK_COUNTER_LOW] =  (*feedback_counter);

  //run current state of the machine
  switch(STATE_BITS & pump_state){
    case 0x00: //idle state

      //exit if enable bit is low
      if( !READBIT(pump_state, ENABLE_BIT) ) 
        break;
      d_println("enable on")
      //Set next State
      pump_state += 1;
      
      //store operational paramaters.
      SETBIT(pump_state, ENABLE_BIT, 1);
      SETBIT(pump_state, FEEDBACK_BIT, c[ COIL_FEEDBACK ] );
      SETBIT(pump_state, DEBUG_BIT,    c[ COIL_DEBUG ] );
      SETBIT(pump_state, INVERT_BIT,   c[ COIL_INVERT ] );
      calibration   = h[  HR_CAL ];
      duration      = h[  HR_DURATION ];
      dose          = h[  HR_DOSE ];
      // value verification
      if( calibration == 0 ){ calibration   = 1; }
      if( duration == 0 )   { duration      = 1; }
      //configure feedback counter
      (*feedback_counter) = 0; //reset counter
      if( READBIT( pump_state, FEEDBACK_BIT ) ){
        enable_feedback(id);
      }else{
        disable_feedback(id);  
      }

      {
        uint64_t total;
        total   = (uint64_t)calibration * (uint64_t)dose;   // TOTAL_FEEDBACK needed to satisfy request = CAL * dose;        
        dose    = total / duration;                         // Total amount to dose per cycle = total/cycles
      
        if( total >= 0x00000000FFFFFFFF ) { //ERROR can not pump more than we can count.
          pump_state |= 0x03;       //enter last state
          i[ DI_ERROR ] = 1;        //set error flag
          break;
        }
        
        if( dose == 0){ dose = 1; } //fix possible of a 0 dose. 
        //store values
        total_feedback  = (uint32_t)total;
        i[ IR_CYCLES ]  = duration;
        i[ IR_DOSE ]    = dose;
      }
      //initalize some data
      timer = 0;
      i[ IR_TOTAL ] = 0;        //set total dispenced to 0
      d[ DI_BUSY ]  = 1;        //set busy flag to 1 
      d[ DI_ERROR ] = 0;        //clear error flag
      motor = GetNewMotor(id);
      if( !motor ){
          pump_state |= 0x03;       //enter last state
          i[ DI_ERROR ] = 1;        //set error flag
          break;
      }else{
        motor->setup_motor(this);
      }
    break;

    case 0x01: //start a pumping cycle
      //exit if its too soon to start another cycle.
      if( millis() - timer <= 1000 )
        break;
      //GoTo last stage if enable is off
      //or the request has been saticfied and debug is false
      if( !READBIT(pump_state, ENABLE_BIT)
      ||
        ( total_feedback <= (*feedback_counter)
        &&
        !READBIT( pump_state, DEBUG_BIT) )
      ){
        pump_state |= 0x03;
        d[ DI_BUSY ] = 0;
        break;
      }

      //save the starting point to determin the stopping point
      start_feedback = (*feedback_counter);  
      //save the time the cycle started.
      timer = millis();
      //update modbus cycle counter register
      if( i[IR_CYCLES] )
        i[ IR_CYCLES ] -= 1;
      //run the motor
      motor->run_motor(this, 255);
      //Set next State
      pump_state += 1;
    break;

    case 0x02: //Stop a pump cycle
      {
        // check if the request has been saticfied 
        // or the dose for this cycle has been reached 
        // or the enable bit is low
        if( total_feedback <= (*feedback_counter) 
        ||
            ((uint32_t)duration - i[ IR_CYCLES ]) * i[ IR_DOSE ] <= (*feedback_counter)
        ||
            !READBIT(pump_state, ENABLE_BIT)
        ){
          if( READBIT( pump_state, DEBUG_BIT) && READBIT(pump_state, ENABLE_BIT))
            break;
          //Stop the pump
          motor->run_motor(this, 0);
          pump_state -=1; //GoTo previous stage
          break;
        }
        motor->work_motor(this);
      }
    break;

    default: //Final stage, lock out the state machine until ENABLE_BIT goes low.
      //Check for reset condition
      if( !READBIT(pump_state, ENABLE_BIT) ){
        d[ DI_ERROR] = 0; //Clear error flag on reset
        pump_state &= ~0x03;
        motor->stop_motor(this);
        delete motor;
        motor = 0;
        break;
      }
  }// End of switch statement
  return;
}