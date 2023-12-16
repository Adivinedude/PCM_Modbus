#ifndef __USE_MODBUS_H__
  #define __USE_MODBUS_H__
  #include "./src/ModbusRTUSlave_modified/ModbusRTUSlave.h"
  #include "register_mapping.h" 
  extern ModbusRTUSlave _modbus;

  void setup_modbus(int address);


#endif