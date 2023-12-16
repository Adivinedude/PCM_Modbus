#include "_Servo_motor.h"
#include "../../globals.h"
#include "Servo.h"
///////////////
//Servo motor//
Servo servos[4];
//short servo_position[4][2];

void Servo_setup(unsigned char motor_number){
  servos[motor_number].attach(MOTOR_PINS[motor_number][MOTOR_ENABLE]);
  //ToDo. use all 3 motor pins to control 3 different servos.
}

void Servo_run(unsigned char motor_number, short angle){
  angle += 0; //remove compiler warning
  servos[motor_number].write( hr_data[ hold_register_total * motor_number + HR_DOSE ] );
}

void Servo_work(unsigned char motor_number){
  //ToDo. add ability to slowly move the server here
  //ToDo. add ability to control more servos here by 
  //        writing the holding registers to the servo library
  motor_number += 0; //removing compiler warning
}

void Servo_stop(unsigned char motor_number){
  servos[motor_number].detach();
}