#ifndef __GLOBALS_H__
  #define __GLOBALS_H_
  #include "sketch_PCM.h"
  #include "pins.h"
  #include "use_modbus.h"
  #include "my_useful_functions.h"
  //extern uint32_t pump_values[4][PUMP_VALUE_SIZE];        //pump states and values
  extern volatile uint32_t pump_counters[4];//pump feedback counters
  extern volatile bool time_to_update;
  //modbus data
  extern bool  coil_data [];
  extern bool  di_data   [];
  extern uint16_t hr_data   [];
  extern uint16_t ir_data   [];
  //motor controller pins.
  extern const unsigned char MOTOR_PINS[][3];

  extern uint16_t _gFreeRam;
  int freeRam();
  void TrackFreeRam();

#endif