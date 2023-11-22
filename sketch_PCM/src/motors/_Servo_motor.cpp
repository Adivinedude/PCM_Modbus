#include "_Servo_motor.h"
#include "../../globals.h"
#include "Servo.h"
///////////////
//Servo motor//
Servo servos[4];
//short servo_position[4][2];

void Servo_setup(unsigned char motor_number){
  servos[motor_number].attach(MOTOR_PINS[motor_number][MOTOR_ENABLE]);
}

void Servo_run(unsigned char motor_number, short angle){
  angle += 0; //remove compiler warning
  servos[motor_number].write( hr_data[ hold_register_total * motor_number + HR_DOSE ] );
}

void Servo_work(unsigned char motor_number){
  //ToDo. add ability to slowly move the server here
  motor_number += 0; //removing compiler warning
}

void Servo_stop(unsigned char motor_number){
  servos[motor_number].detach();
}