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
    IR_FEEDBACK_COUNTER = 0, // Current value of feedback pulses
    IR_CYCLES,          // How many 1 second cycles left until the request is complete
    IR_DOSE,            // How many feedback pulses be dispenced per cycle
    IR_TIMER_VALUE,     // Time the last cycle started
    IR_TOTAL,           // Total amount dispenced in units.
    input_register_total
  };

  //Modbus address offsets for each pump
  //coils - controler out, server in
  enum pump_offset_coil{
    PUMP_1_C = 0,
    PUMP_2_C = coil_total,
    PUMP_3_C = coil_total * 2,
    PUMP_4_C = coil_total * 3
  };
  //descrete inputs controler in, server out
  enum pump_offset_di{
    PUMP_1_D = 0,
    PUMP_2_D = descrete_input_total,
    PUMP_3_D = descrete_input_total * 2,
    PUMP_4_D = descrete_input_total * 3
  };
  //hold registers controler out, server in
  enum pump_offset_hr{
    PUMP_1_H = 0,
    PUMP_2_H = hold_register_total,
    PUMP_3_H = hold_register_total * 2,
    PUMP_4_H = hold_register_total * 3
  };
  //input registers, controler in, server out
  enum pump_offset_ir{
    PUMP_1_I = 0,
    PUMP_2_I = input_register_total,
    PUMP_3_I = input_register_total * 2,
    PUMP_4_I = input_register_total * 3
  };
  //special input register at the end
  enum special_registers{
    IR_FREE_RAM = input_register_total * 4,  // report system memory
    IR_LOOP_SPEED                             // report system speed
  };

  //value stored within the pump_values array
  enum pump_values_meaning {
    PUMP_STATE = 0,   //current state of the machine
    PUMP_CAL,         //current command paramaters
    PUMP_DURATION,    //how long to dispence set amount
    PUMP_DOSE,        //
    PUMP_CYCLE_START_FEEDBACK, //Feedback counter value at start of current cycle
    PUMP_VALUE_SIZE
  };

  //bits of pump_values[PUMP_STATE] register
  enum pump_state_bits{
    STATE_BIT0 = 0,
    STATE_BIT1,
    MOTOR_BIT0,
    MOTOR_BIT1,
    MOTOR_BIT2,
    MOTOR_BIT3,
    UNUSED_BIT6,
    UNUSED_BIT7,
    UNUSED_BIT8,
    UNUSED_BIT9,
    UNUSED_BIT10,
    UNUSED_BIT11,
    INVERT_BIT,
    DEBUG_BIT,
    FEEDBACK_BIT,
    ENABLE_BIT
  };

  #define MOTOR_BITS    0x3C //type of motor
  #define STATE_BITS    0x03 //current state
  //unused bit 11 - 4
  enum motor_types{
    MOTOR_TYPE_DC = 0,
    MOTOR_TYPE_SERVO,
    MOTOR_TYPE_STEPPER,
    MOTOR_TYPE_PULSE_STEP
  };
#endif