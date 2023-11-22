#include "_DC_Motor.h"
#include "../../globals.h"
////////////
//Dc Motor//

void DCMotor_run(unsigned char pump_number, short on){
  unsigned char en, ina, inb;
  en  = MOTOR_PINS[pump_number][MOTOR_ENABLE];
  ina = MOTOR_PINS[pump_number][MOTOR_A];
  inb = MOTOR_PINS[pump_number][MOTOR_B];

  digitalWrite( en, LOW );  //disable motor before making any changes.
  //stop motor
  if( on == 0){
    digitalWrite( ina, LOW );
    digitalWrite( inb, LOW );
    return;
  }
  // Invert pump direction if told to
  if( READBIT(pump_values[pump_number][PUMP_STATE], INVERT_BIT) ){
    on *= -1;
  }
  // validate input
  if( on > 255 ) on = 255;
  if( on < -255) on = -255;
  
  if( on > 0){
    digitalWrite( inb, LOW ); //run motor foward
    digitalWrite( ina, HIGH );
  
  }else{
    digitalWrite( ina, LOW );//run motor reverse
    digitalWrite( inb, HIGH );
  }
  analogWrite( en, abs(on) );
  return;
}

void DCMotor_work(unsigned char motor_number){
  //Store feedback if physical feedback is not used.
  if( !READBIT(pump_values[motor_number][PUMP_STATE], FEEDBACK_BIT) )
    pump_counters[motor_number] = 
        millis() 
      - ir_data[ input_register_total * motor_number + IR_TIMER_VALUE ] 
      + pump_values[motor_number][PUMP_CYCLE_START_FEEDBACK];
}