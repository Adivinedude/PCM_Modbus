#ifndef ___SERVO_MOTOR_H__
  #define ___SERVO_MOTOR_H__
  void Servo_setup(unsigned char motor_number);
  void Servo_run(unsigned char motor_number, short);
  void Servo_work(unsigned char motor_number);
  void Servo_stop(unsigned char motor_number);
#endif