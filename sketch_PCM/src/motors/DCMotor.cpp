#include "DCMotor.h"
#include "../../cPCM.h"
#include "../../globals.h"

/*//////////////////////////////////////////////////////////
//                  Built to control the                  //
//           L298 Series of Motor Driver Module           //
// https://dronebotworkshop.com/dc-motors-l298n-h-bridge/ //
//////////////////////////////////////////////////////////*/

void cDCMotor::setup_motor(cPCM* t){
  d_println("cDCMotor::setup")
  start_feedback = 0;
}
void cDCMotor::run_motor(cPCM* t, uint8_t on){
  unsigned char en, ina, inb;
  en  = MOTOR_PINS[t->id][MOTOR_ENABLE];
  ina = MOTOR_PINS[t->id][MOTOR_A];
  inb = MOTOR_PINS[t->id][MOTOR_B];

  digitalWrite( en, LOW );  //disable motor before making any changes.
  //stop motor
  if( on == 0){
    d_println("stop motor")
    start_feedback = *t->feedback_counter;
    digitalWrite( ina, LOW );
    digitalWrite( inb, LOW );
    return;
  }
  cycle_start_time = millis();
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
  d_println("run motor")
  analogWrite( en, on );
}

void cDCMotor::work_motor(cPCM* t){
 //Store feedback if physical feedback is not used.
  if( !READBIT(t->pump_state, FEEDBACK_BIT) )
    *t->feedback_counter = (millis() - cycle_start_time) + start_feedback;
}