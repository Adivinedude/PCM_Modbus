#ifndef STEPPER_MOTOR_H
  #define STEPPER_MOTOR_H
  #include "motor_driver.h"
  #include "../Stepper_compact/Stepper_compact.h"
  class cStepperMotor : public cMotor_Base_Class{
    void setup_motor(cPCM*) override;
    void run_motor  (cPCM*, uint8_t) override;
    void work_motor (cPCM*) override;
    void stop_motor (cPCM*) override;
    //void stop_motor (cPCM*);
    Stepper stepper;
  };
#endif