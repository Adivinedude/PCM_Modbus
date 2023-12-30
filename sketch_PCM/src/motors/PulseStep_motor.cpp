#include "PulseStep_motor.h"
#include "../../globals.h"
#include "../../cPCM.h"

/*////////////////////////////////////
//       Built to control the       //
// gecko g540 4-axis stepper driver //
//   https://www.geckodrive.com/    //
////////////////////////////////////*/

void cPulseStep::setup_motor(cPCM* t){
  //setup the stepper in a disabled state
  delay = 0;
  //data sheet page 2 requires 200ns after switching the direction bit before the next step
  timer = micros() + 1; //add 1000ns before the next step. smallest ammount we can add
  digitalWrite(MOTOR_PINS[t->id][MOTOR_ENABLE], LOW);
  digitalWrite(MOTOR_PINS[t->id][MOTOR_A], !READBIT(t->pump_state, INVERT_BIT));
  if( !READBIT(t->pump_state,FEEDBACK_BIT) )
    t->feedback_counter = 0;
}
//data sheet pg 2 requires step pin pulses to be 2us 'LOW' 1us 'HIGH'
//ensure this is enforced by setting the minimum delay to 2us
//set the delay so steps per second == cycle dose;
void cPulseStep::run_motor(cPCM* t, uint8_t speed){
  //stop motor if speed == 0
  if(speed == 0){
    delay = 0;
    digitalWrite(MOTOR_PINS[t->id][MOTOR_ENABLE], LOW);
    return;
  }
  //set speed to match cycle's dose
  //the delay inbetween pin toggles needs to be halfed to produce a 50% duty cycle at desired step frequency
  delay = 1000L * 1000L / t->i[IR_DOSE] / 2;
  //ensure we do not violate the datasheet timing requirments.
  if(delay < 4)
    delay = 4;
}

void cPulseStep::work_motor(cPCM* t){
  //exit if its not time to make the next step or the motor is stopped
  if( (delay == 0) || ((micros() - timer) < delay) )
    return;
  timer = micros();
  //invert the pin state stored in pinstate
  pinstate = !pinstate;
  //write the pin state stored in pinstate to the pin.
  digitalWrite(MOTOR_PINS[t->id][MOTOR_ENABLE], pinstate);
  //If using internal feedback, increment counter on falling edge.
  if( !pinstate &&  !READBIT(t->pump_state,FEEDBACK_BIT) )
    pump_counters[t->id]++;
}

void cPulseStep::stop_motor(cPCM* t){
  delay = 0;
  digitalWrite(MOTOR_PINS[t->id][MOTOR_ENABLE], LOW);
}