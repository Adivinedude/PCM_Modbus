#include "Servo_motor.h"
#include "../../globals.h"
#include "../../cPCM.h"
///////////////
//Servo motor//
//short servo_position[4][2];

void cServoMotor::setup_motor(cPCM* t){
  servos[0].attach(MOTOR_PINS[t->id][MOTOR_ENABLE]);
  servos[1].attach(MOTOR_PINS[t->id][MOTOR_A]);
  servos[2].attach(MOTOR_PINS[t->id][MOTOR_B]);
  t->c[COIL_DEBUG] = 1;
}
void cServoMotor::run_motor(cPCM* t, uint8_t){
}
void cServoMotor::work_motor(cPCM* t){
  servos[0].write( constrain(t->h[HR_CAL],      0, 180) ); 
  servos[1].write( constrain(t->h[HR_DURATION], 0, 180) );
  servos[2].write( constrain(t->h[HR_DOSE],     0, 180) );
}
void cServoMotor::stop_motor(cPCM*){
  servos[0].detach();
  servos[1].detach();
  servos[2].detach();
}