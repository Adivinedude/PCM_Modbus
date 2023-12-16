#include "pins.h"
#include <Arduino.h>

void setup_pins(){
  pinMode(FEEDBACK_ONE, INPUT_PULLUP);
  pinMode(FEEDBACK_TWO, INPUT_PULLUP);
  pinMode(FEEDBACK_THREE, INPUT_PULLUP);
  pinMode(FEEDBACK_FOUR, INPUT_PULLUP);
  //#warning "LED Pin Active for debug"
  //pinMode(FEEDBACK_FOUR, OUTPUT);

  pinMode(MOTOR_ONE_ENABLE, OUTPUT);
  digitalWrite(MOTOR_ONE_ENABLE, LOW);
  pinMode(MOTOR_ONE_IN_A, OUTPUT);
  pinMode(MOTOR_ONE_IN_B, OUTPUT);

  pinMode(MOTOR_TWO_ENABLE, OUTPUT);
  digitalWrite(MOTOR_TWO_ENABLE, LOW);
  pinMode(MOTOR_TWO_IN_A, OUTPUT);
  pinMode(MOTOR_TWO_IN_B, OUTPUT);

  pinMode(MOTOR_THREE_ENABLE, OUTPUT);
  digitalWrite(MOTOR_THREE_ENABLE, LOW);
  pinMode(MOTOR_THREE_IN_A, OUTPUT);
  pinMode(MOTOR_THREE_IN_B, OUTPUT);

  pinMode(MOTOR_FOUR_ENABLE, OUTPUT);
  digitalWrite(MOTOR_FOUR_ENABLE, LOW);
  pinMode(MOTOR_FOUR_IN_A, OUTPUT);
  pinMode(MOTOR_FOUR_IN_B, OUTPUT);

  pinMode(RS485_TX, OUTPUT);
  pinMode(RS485_RX, INPUT);
  pinMode(RS485_DERE, OUTPUT);
  digitalWrite(RS485_DERE, HIGH);
}