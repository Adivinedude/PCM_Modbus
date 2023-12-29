#include "globals.h"
#include "register_mapping.h"

//feedback counters for each pump 
volatile uint32_t pump_counters[MAX_NUM_PUMP];     
volatile bool time_to_update = false;
//modbus data
bool     coil_data [MAX_NUM_PUMP * coil_total];
bool     di_data   [MAX_NUM_PUMP * descrete_input_total];
uint16_t hr_data   [MAX_NUM_PUMP * hold_register_total];
uint16_t ir_data   [MAX_NUM_PUMP * input_register_total + special_input_register_size];

//motor controller pins.
const unsigned char MOTOR_PINS[4][3] = {{MOTOR_ONE_ENABLE, MOTOR_ONE_IN_A, MOTOR_ONE_IN_B}, 
                                        {MOTOR_TWO_ENABLE, MOTOR_TWO_IN_A, MOTOR_TWO_IN_B},
                                        {MOTOR_THREE_ENABLE, MOTOR_THREE_IN_A, MOTOR_THREE_IN_B},
                                        {MOTOR_FOUR_ENABLE, MOTOR_FOUR_IN_A, MOTOR_FOUR_IN_B}};

 int freeRam(){ 
    extern int __heap_start,
               *__brkval; 
    unsigned int a;
    return (int)&a - (__brkval==0 ? (int)&__heap_start : (int) __brkval);
}
uint16_t _gFreeRam = 0xFFFF;
void TrackFreeRam(){
    uint16_t t;
    if( (t = freeRam()) < _gFreeRam)
        _gFreeRam = t;
}                                       