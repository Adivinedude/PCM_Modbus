#ifndef MOTOR_DRIVER_H
  #define MOTOR_DRIVER_H
  #include <stdint.h>
  class cPCM;
  //modbus data array. saves keystrokes and makes the code look cleaner.
  class cMotor_Base_Class {
    public:
        cMotor_Base_Class(){};
        virtual ~cMotor_Base_Class(){};
        virtual bool UseDefaultHandler() { return true;}
        virtual void setup_motor(cPCM*){};
        virtual void run_motor  (cPCM*, uint8_t speed){};
        virtual void work_motor (cPCM*){}
        virtual void stop_motor (cPCM*){}
};

#endif