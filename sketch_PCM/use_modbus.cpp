#include "use_modbus.h"
#include "pins.h"
#include "globals.h"

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
  ModbusRTUSlave _modbus(Serial, RS485_DERE);
#endif
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
  ModbusRTUSlave _modbus(Serial1, RS485_DERE);
#endif

void setup_modbus(int address){
  //configure ModBus Server.
  // start the Modbus RTU server, with server id 'address'
  _modbus.begin(address, 9600);
  _modbus.configureCoils(           coil_data,  coil_total          * MAX_NUM_PUMP);
  _modbus.configureDiscreteInputs(  di_data,    descrete_input_total* MAX_NUM_PUMP);
  _modbus.configureHoldingRegisters(hr_data,    hold_register_total * MAX_NUM_PUMP);
  _modbus.configureInputRegisters(  ir_data,    input_register_total* MAX_NUM_PUMP + 2); //Plus special registers

  for(int i = 0; i != coil_total * MAX_NUM_PUMP; i++)
    coil_data[i] = 0;

  for(int i = 0; i != descrete_input_total * MAX_NUM_PUMP; i++)
    di_data[i] = 0;

  for(int i = 0; i != hold_register_total * MAX_NUM_PUMP; i++)
    hr_data[i] = 0;

  for(int i = 0; i != input_register_total * MAX_NUM_PUMP + 2; i++)
    ir_data[i] = 0;
}
