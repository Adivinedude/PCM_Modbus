#include "_motor_drivers.h"
#include "../../globals.h"

//Supported Motors
#include "_DC_Motor.h"
#include "_Servo_motor.h"
#include "_Stepper_motor.h"
#include "_PulseStep_motor.h"

void setup_motor(unsigned char motor_number){
  d_print("motor start:");d_println((pump_values[motor_number][PUMP_STATE] & MOTOR_BITS) >> 2);
  switch( (pump_values[motor_number][PUMP_STATE] & MOTOR_BITS) >> 2 ){
    /*case MOTOR_TYPE_DC:
      break;*/
    case MOTOR_TYPE_SERVO:
      Servo_setup(motor_number);
      break;
    case MOTOR_TYPE_STEPPER:
      Stepper_setup(motor_number);
      break;
    case MOTOR_TYPE_PULSE_STEP:
      PulseStep_setup(motor_number);
      break;
  }
}

void run_motor(unsigned char motor_number, short speed){ 
  switch( (pump_values[motor_number][PUMP_STATE] & MOTOR_BITS) >> 2 ){
    case MOTOR_TYPE_DC:
      DCMotor_run(motor_number, speed);
      break;
    case MOTOR_TYPE_SERVO:
      Servo_run(motor_number, speed);
      break;
    case MOTOR_TYPE_STEPPER:
      Stepper_run(motor_number, speed);
      break;
    case MOTOR_TYPE_PULSE_STEP:
      PulseStep_run(motor_number, speed);
      break;
  }
}

void work_motor(unsigned char motor_number){
  switch( (pump_values[motor_number][PUMP_STATE] & MOTOR_BITS) >> 2 ){
    case MOTOR_TYPE_DC:
      DCMotor_work(motor_number);
      break;
    case MOTOR_TYPE_SERVO:
      Servo_work(motor_number);
      break;
    case MOTOR_TYPE_STEPPER:
      Stepper_work(motor_number);
      break;
    case MOTOR_TYPE_PULSE_STEP:
      PulseStep_work(motor_number);
      break;
  }
}

void stop_motor(unsigned char motor_number){
  switch( (pump_values[motor_number][PUMP_STATE] & MOTOR_BITS) >> 2 ){
    /*case MOTOR_TYPE_DC:
      break;*/
    case MOTOR_TYPE_SERVO:
      Servo_stop(motor_number);
      break;
    /*case MOTOR_TYPE_STEPPER:
      break;*/
    case MOTOR_TYPE_PULSE_STEP:
      PulseStep_stop(motor_number);
      break;
  }
}