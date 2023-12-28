#include "Stepper_motor.h"
#include "../../globals.h"
#include "../../cPCM.h"
/////////////////
//Stepper Motor//
void cStepperMotor::setup_motor(cPCM* t){
  Stepper * pS;
  switch( t->id ){
    case 0:
      stepper.Setup( t->calibration, MOTOR_ONE_IN_A, MOTOR_ONE_IN_B, MOTOR_TWO_IN_A, MOTOR_TWO_IN_B);
    break;

    case 2:
      stepper.Setup( t->calibration, MOTOR_THREE_IN_A, MOTOR_THREE_IN_B, MOTOR_FOUR_IN_A, MOTOR_FOUR_IN_B);
    break;

    default:
      //invalid motor selected. motor 1 or 3
      t->d[DI_ERROR] = 1; //set error flag
      t->pump_state |= 0x03; //lockout pump;
      return;
  }
  pS->setSteps( t->i[IR_DOSE] );
  if( !t->c[COIL_FEEDBACK] )
    pS->_feedback = t->feedback_counter;
}

void cStepperMotor::run_motor(cPCM* t, uint8_t steps){ 
  stepper.nonblocking_step( steps );
}

void cStepperMotor::work_motor(cPCM*){
  stepper.update();
}
