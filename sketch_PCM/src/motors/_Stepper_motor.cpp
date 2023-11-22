#include "_Stepper_motor.h"
#include "../../globals.h"
#include "Stepper_compact.h"

/////////////////
//Stepper Motor//
Stepper steppers[2];
void Stepper_setup(unsigned char motor_number){
  Stepper * pS;
  switch( motor_number ){
    case 0:
      pS = &steppers[0];
      //Setup ( steps per unit, pins)
      pS->Setup( pump_values[motor_number][PUMP_CAL], MOTOR_ONE_IN_A, MOTOR_ONE_IN_B, MOTOR_TWO_IN_A, MOTOR_TWO_IN_B);
    break;

    case 2:
      pS = &steppers[1];
      //Setup ( steps per unit, pins)
      pS->Setup( pump_values[motor_number][PUMP_CAL], MOTOR_THREE_IN_A, MOTOR_THREE_IN_B, MOTOR_FOUR_IN_A, MOTOR_FOUR_IN_B);
    break;

    default:
      //invalid motor selected. motor 1 or 3
      di_data[ motor_number * descrete_input_total + DI_ERROR] = 1; //set error flag
      pump_values[motor_number][PUMP_STATE] |= 0x03; //lockout pump;
      return;
  }
  pS->setSteps( ir_data[ motor_number * input_register_total + IR_DOSE ] );
  if( !coil_data[ coil_total * motor_number + COIL_FEEDBACK ] )
    pS->_feedback = &pump_counters[motor_number];
}

void Stepper_run(unsigned char motor_number, short steps){ 
  steppers[motor_number/2].nonblocking_step( steps );
}

void Stepper_work(unsigned char motor_number){
  steppers[motor_number/2].update();
}
