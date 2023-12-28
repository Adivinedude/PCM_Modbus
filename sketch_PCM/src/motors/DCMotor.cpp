#include "DCMotor.h"
#include "../../cPCM.h"
#include "../../globals.h"
////////////
//Dc Motor//
void cDCMotor::run_motor(cPCM* t, uint8_t on){
  unsigned char en, ina, inb;
  en  = MOTOR_PINS[t->id][MOTOR_ENABLE];
  ina = MOTOR_PINS[t->id][MOTOR_A];
  inb = MOTOR_PINS[t->id][MOTOR_B];

  digitalWrite( en, LOW );  //disable motor before making any changes.
  //stop motor
  if( on == 0){
    digitalWrite( ina, LOW );
    digitalWrite( inb, LOW );
    return;
  }
  switch (READBIT(t->pump_state, INVERT_BIT)){
    case false:
      digitalWrite( inb, LOW ); //run motor foward
      digitalWrite( ina, HIGH );
      break;
    case true:
      digitalWrite( ina, LOW );//run motor reverse
      digitalWrite( inb, HIGH );
      break;
  }
  analogWrite( en, on );
}

void cDCMotor::work_motor(cPCM* t){
  //Store feedback if physical feedback is not used.
  if( !READBIT(t->pump_state, FEEDBACK_BIT) )
    pump_counters[t->id] = millis() - (t->timer) + t->start_feedback;
}