#ifndef PULSESTEP_MOTOR_H
  #define PULSESTEP_MOTOR_H
  #include "motor_driver.h"

  class cPulseStep: public cMotor_Base_Class{
    void      setup_motor (cPCM*);
    void      run_motor   (cPCM*, uint8_t);
    void      work_motor  (cPCM*);
    void      stop_motor  (cPCM*);
    uint32_t  timer;
    uint32_t  delay;
    uint8_t   pinstate;
  };
#endif