#ifndef ___STEPPER_MOTOR_H__
  #define ___STEPPER_MOTOR_H__
  void Stepper_setup(unsigned char motor_number);
  void Stepper_run(unsigned char motor_number, short);
  void Stepper_work(unsigned char motor_number);
  void Stepper_stop(unsigned char motor_number);
#endif