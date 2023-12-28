#ifndef __REGISTER_MAPPING_H__
  #define __REGISTER_MAPPING_H__

  #define MAX_NUM_PUMP 4

  //coils
  enum coil_names {
    COIL_ENABLE = 0,  //is the pump on=1 or off=0
    COIL_FEEDBACK,    //using physical feedback=1 or internal feedback=0
    COIL_DEBUG,       //direct control of pump. pump runs when debug=1 && enable=1
    COIL_INVERT,      //invert direction of pump
    coil_total
  };

  //descrete input
  enum descrete_input{
    DI_BUSY = 0,      // 0=waiting for command, 1=working on command
    DI_ERROR,         // 0=normal 1=Invalid data values where entered.
    descrete_input_total
  };

  //hold registers
  enum hold_registers{
    HR_CAL = 0,         // How many feedback pulses equals 1 unit
    HR_DURATION,        // Minimume time to take to despence requested amount
    HR_DOSE,            // How many units to dispence
    HR_MOTOR_TYPE,      // 0 = DC, 1 = Servo, 2 = Stepper(bipolar), 4 = Pulse Step
    hold_register_total
  };

  //input registers
  enum input_registers{
    IR_FEEDBACK_COUNTER_HIGH = 0, // Current value of feedback pulses
    IR_FEEDBACK_COUNTER_LOW,
    IR_CYCLES,          // How many 1 second cycles left until the request is complete
    IR_DOSE,            // How many feedback pulses be dispenced per cycle
    IR_TOTAL,           // Total amount dispenced in units.
    input_register_total
  };

  //Modbus address offsets for each pump
  //special input register at the end
  enum special_input_registers{
    IR_FREE_RAM = 0,  // report system memory
    IR_LOOP_SPEED,    // report system speed
    IR_CLOCK,         // report system up time
    special_input_register_size
  };

  //value stored within the pump_values array
  enum pump_values_meaning {
    PUMP_STATE = 0,   //current state of the machine
    PUMP_CAL,         //current command paramaters
    PUMP_DURATION,    //how long to dispence set amount
    PUMP_DOSE,        //
    PUMP_CYCLE_START_FEEDBACK, //Feedback counter value at start of current cycle
    PUMP_TOTAL_FEEDBACK,
    PUMP_TIMER_VALUE,
    PUMP_VALUE_SIZE
  };

  //bits of pump_values[PUMP_STATE] register
  enum pump_state_bits{
    STATE_BIT0 = 0,
    STATE_BIT1,
    UNUSED_BIT2,
    UNUSED_BIT3,
    INVERT_BIT,
    DEBUG_BIT,
    FEEDBACK_BIT,
    ENABLE_BIT
  };

  #define STATE_BITS    0x03 //current state
  enum motor_types{
    MOTOR_TYPE_DC = 0,
    MOTOR_TYPE_SERVO,
    MOTOR_TYPE_STEPPER,
    MOTOR_TYPE_PULSE_STEP
  };
#endif