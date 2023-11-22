#ifndef ___PULSESTEP_H__
  #define ___PULSESTEP_H__
  void PulseStep_setup(unsigned char motor_number);
  void PulseStep_run(unsigned char motor_number, short);
  void PulseStep_work(unsigned char motor_number);
  void PulseStep_stop(unsigned char motor_number);
#endif