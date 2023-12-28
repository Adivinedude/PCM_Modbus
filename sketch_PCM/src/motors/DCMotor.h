#ifndef DCMOTOR_H
#define DCMOTOR_H
  #include "motor_driver.h"
  class cDCMotor: public cMotor_Base_Class{
    void run_motor    (cPCM*, uint8_t speed);
    void work_motor   (cPCM*);
  };  
#endif