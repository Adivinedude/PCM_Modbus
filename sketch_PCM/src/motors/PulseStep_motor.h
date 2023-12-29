#ifndef PULSESTEP_MOTOR_H
  #define PULSESTEP_MOTOR_H
  #include "motor_driver.h"

  class cPulseStep: public cMotor_Base_Class{
    void      setup_motor (cPCM*) override;
    void      run_motor   (cPCM*, uint8_t) override;
    void      work_motor  (cPCM*) override;
    void      stop_motor  (cPCM*) override;
    uint32_t  timer;
    uint32_t  delay;
    uint8_t   pinstate;
  };
#endif