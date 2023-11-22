#include "globals.h"
#include "register_mapping.h"

//storage for the state of each pump
uint16_t pump_values[4][5] = {{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};

//feedback counters for each pump 
volatile uint16_t pump_counters[4] = {0,0,0,0};     

//modbus data
bool  coil_data [MAX_NUM_PUMP * coil_total];
bool  di_data   [MAX_NUM_PUMP * descrete_input_total];
uint16_t hr_data   [MAX_NUM_PUMP * hold_register_total];
uint16_t ir_data   [MAX_NUM_PUMP * input_register_total + 2];

//motor controller pins.
const unsigned char MOTOR_PINS[4][3] = {{MOTOR_ONE_ENABLE, MOTOR_ONE_IN_A, MOTOR_ONE_IN_B}, 
                                        {MOTOR_TWO_ENABLE, MOTOR_TWO_IN_A, MOTOR_TWO_IN_B},
                                        {MOTOR_THREE_ENABLE, MOTOR_THREE_IN_A, MOTOR_THREE_IN_B},
                                        {MOTOR_FOUR_ENABLE, MOTOR_FOUR_IN_A, MOTOR_FOUR_IN_B}};