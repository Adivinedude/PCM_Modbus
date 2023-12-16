#include "feedback.h"
#include <Arduino.h>
#include "globals.h"
#include "my_useful_functions.h"
volatile unsigned char pinb_pin_state = 0; //previous values from portb
void setup_feedback(){
  PCMSK0 = B00000000; //clear pins that will fire the ISR PCINT7-0
  PCICR |= B00000001; //enable Pin Change Interrupts on PORTB
  //Store port state.
  pinb_pin_state = PINB;
}

void enable_feedback(unsigned char pump_number){
  //set the Interrupt to fire on this pin
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
    SETBIT(PCMSK0, pump_number, 1);
  #endif
  #if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
    SETBIT(PCMSK0, (pump_number + 2), 1);
  #endif
}

void disable_feedback(unsigned char pump_number){
  //disable the Interrupt for this pin
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
    SETBIT(PCMSK0, pump_number, 0); 
  #endif
  #if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
    SETBIT(PCMSK0, (pump_number + 2), 0);
  #endif    
}

//pin change hardware intrupte routine for physical feedback PORTB
ISR (PCINT0_vect){
  //ISR for PORTB
  unsigned char temp = pinb_pin_state;
  //detect change in pin state, XOR the previously stored values with the current values [0001 ^ 0100 == 0101]
  temp ^= PINB;
  // 'AND' this list with a list that is capable of firing the interrupt  [1100 & 0101 == 0100]
  temp &= PCMSK0;

  //corrent for the different pinout of the arduino uno
  #if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
    temp >>= 1;
  #endif

  // increment counters that changed states
  pump_counters[0] += temp & 0x01;  
  temp >>= 1;                       
  pump_counters[1] += temp & 0x01;
  #if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
    temp >>= 2;
  #else
    temp >>= 1;
  #endif
  pump_counters[2] += temp & 0x01;
  temp >>= 1;
  pump_counters[3] += temp & 0x01;
  // store new pin states
  pinb_pin_state = PINB;
}