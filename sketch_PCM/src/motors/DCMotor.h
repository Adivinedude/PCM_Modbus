#ifndef DCMOTOR_H
#define DCMOTOR_H
  #include "motor_driver.h"
  class cDCMotor: public cMotor_Base_Class{
    void setup_motor(cPCM*) override;
    void run_motor (cPCM*, uint8_t speed) override;
    void work_motor(cPCM*) override;
    uint32_t start_feedback;
    uint32_t cycle_start_time;
  };  
#endif