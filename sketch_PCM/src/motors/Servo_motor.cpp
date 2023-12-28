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
}

void cServoMotor::run_motor(cPCM* t, uint8_t){
  servos[0].write( t->h[HR_CAL] );
  servos[1].write( t->h[HR_DURATION]);
  servos[3].write( t->h[HR_DOSE]);
}

void cServoMotor::work_motor(cPCM* t){
}

void cServoMotor::stop_motor(cPCM*){
  servos[0].detach();
  servos[1].detach();
  servos[2].detach();
}