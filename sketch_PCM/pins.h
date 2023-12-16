#ifndef __PINS_H__
  #define __PINS_H__
  void setup_pins();

  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
    //Pins for Arduino Micro
    #define LED_PIN             13  // PC7
    //PCInt pins, atmega32u4 only supports portb
    #define FEEDBACK_ONE        17 // PB0
    #define FEEDBACK_TWO        15 // PB1
    #define FEEDBACK_THREE      16 // PB2
    #define FEEDBACK_FOUR       14 // PB3

    #define MOTOR_ONE_ENABLE    3  // PD0
    #define MOTOR_ONE_IN_A      2  // PD1
    #define MOTOR_ONE_IN_B      4  // PD4

    #define MOTOR_TWO_ENABLE    5  // PC6
    #define MOTOR_TWO_IN_A      7  // PE6
    #define MOTOR_TWO_IN_B      8  // PB4

    #define MOTOR_THREE_ENABLE  6  // PD7
    #define MOTOR_THREE_IN_A    10 // PB6
    #define MOTOR_THREE_IN_B    9  // PB5

    #define MOTOR_FOUR_ENABLE   11 // PB7
    #define MOTOR_FOUR_IN_A     12 // PD6
    #define MOTOR_FOUR_IN_B     13 // PC7

    #define RS485_TX            1  // PD3
    #define RS485_RX            0  // PD2
    #define RS485_DERE          18 // A0  PF7

    #define POT_IN              19 // A5 PC5
  #endif

  #if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
  //Pins for Arduino Uno R3
    #define LED_PIN             13  // PB5
    //PCInt pins
    #define FEEDBACK_ONE        9  // PB1
    #define FEEDBACK_TWO        10 // PB2
    #define FEEDBACK_THREE      12 // PB4
    #define FEEDBACK_FOUR       13 // PB5

    #define MOTOR_ONE_ENABLE    3  // PD3
    #define MOTOR_ONE_IN_A      2  // PD2
    #define MOTOR_ONE_IN_B      4  // PD4

    #define MOTOR_TWO_ENABLE    5  // PD5
    #define MOTOR_TWO_IN_A      7  // PD7
    #define MOTOR_TWO_IN_B      8  // PB0

    #define MOTOR_THREE_ENABLE  6  // PD6
    #define MOTOR_THREE_IN_A    14 // PC0
    #define MOTOR_THREE_IN_B    15 // PC1

    #define MOTOR_FOUR_ENABLE   11 // PB3
    #define MOTOR_FOUR_IN_A     16 // PC2
    #define MOTOR_FOUR_IN_B     17 // PC3

    #define RS485_TX            1  // PD1
    #define RS485_RX            0  // PD0
    #define RS485_DERE          18 // A4 PC4

    #define POT_IN              19 // A5 PC5
  #endif

  //Motor Pin Offsets
  #define MOTOR_ENABLE  0
  #define MOTOR_A       1
  #define MOTOR_B       2

#endif