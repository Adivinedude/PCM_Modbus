#ifndef SERVO_MOTOR_H
  #define SERVO_MOTOR_H
  #include "motor_driver.h"
  #include "Servo.h"

  class cServoMotor : public cMotor_Base_Class{
    public:
      Servo servos[3];
      void setup_motor(cPCM*);
      void run_motor(cPCM*, uint8_t);
      void work_motor(cPCM*);
      void stop_motor(cPCM*);
  };
#endif