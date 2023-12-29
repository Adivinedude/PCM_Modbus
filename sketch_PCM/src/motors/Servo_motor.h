#ifndef SERVO_MOTOR_H
  #define SERVO_MOTOR_H
  #include "motor_driver.h"
  #include "Servo.h"

  class cServoMotor : public cMotor_Base_Class{
    public:
      Servo servos[3];
      bool UseDefaultHandler() override {return false;}
      void setup_motor(cPCM*) override;
      void run_motor(cPCM*, uint8_t) override;
      void work_motor(cPCM*) override;
      void stop_motor(cPCM*) override;
  };
#endif