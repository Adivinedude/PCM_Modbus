#ifndef ___MOTOR_DRIVERS_H__
  #define ___MOTOR_DRIVERS_H__
  
  void setup_motor(unsigned char motor_number);

  void run_motor(unsigned char motor_number, short speed);

  void work_motor(unsigned char motor_number);

  void stop_motor(unsigned char motor_number);

#endif