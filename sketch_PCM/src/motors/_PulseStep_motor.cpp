#include "_PulseStep_motor.h"
#include "../../globals.h"

/////////////////////////
//Pulse Step Motor Controller//
unsigned long timers[4] = {0,0,0,0};
unsigned long delays[4] = {0,0,0,0};
unsigned char pinstate = 0;

void PulseStep_setup(unsigned char motor_number){
  //setup the stepper in a disabled state
  delays[motor_number] = 0;
  //data sheet page 2 requires 200ns after switching the direction bit before the next step
  timers[motor_number] = micros() + 1; //add 1000ns before the next step. smallest ammount we can add
  digitalWrite(MOTOR_PINS[motor_number][0], LOW);
  digitalWrite(MOTOR_PINS[motor_number][1], !READBIT(pump_values[motor_number][PUMP_STATE], INVERT_BIT));
  if( !READBIT(pump_values[motor_number][PUMP_STATE],FEEDBACK_BIT) )
    pump_counters[motor_number] = 0;
}
//data sheet pg 2 requires step pin pulses to be 2us 'LOW' 1us 'HIGH'
//ensure this is enforced by setting the minimum delay to 2us
//set the delay so steps per second == cycle dose;
void PulseStep_run(unsigned char motor_number, short speed){
  //stop motor if speed == 0
  if(speed == 0){
    delays[motor_number] = 0;
    digitalWrite(MOTOR_PINS[motor_number][0], LOW);
    return;
  }
  //set speed to match cycle's dose
  //the delay inbetween pin toggles needs to be halfed to produce a 50% duty cycle at desired step frequency
  delays[motor_number] = 1000L * 1000L / ir_data[ motor_number * input_register_total + IR_DOSE ] / 2;
  //ensure we do not violate the datasheet timing requirments.
  if(delays[motor_number] < 4)
    delays[motor_number] = 4;
}

void PulseStep_work(unsigned char motor_number){
  //exit if its not time to make the next step or the motor is stopped
  if( (delays[motor_number] == 0) || ((micros() - timers[motor_number]) < delays[motor_number]) )
    return;
  timers[motor_number] = micros();
  //invert the pin state stored in pinstate
  SETBIT(pinstate, motor_number, !READBIT(pinstate, motor_number));
  //write the pin state stored in pinstate to the pin.
  digitalWrite(MOTOR_PINS[motor_number][0], READBIT(pinstate, motor_number));
  //If using internal feedback, increment counter on falling edge.
  if( !READBIT(pinstate, motor_number) &&  !READBIT(pump_values[motor_number][PUMP_STATE],FEEDBACK_BIT) )
    pump_counters[motor_number]++;
}

void PulseStep_stop(unsigned char motor_number){
  delays[motor_number] = 0;
  digitalWrite(MOTOR_PINS[motor_number][0], LOW);
}