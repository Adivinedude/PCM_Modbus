# PCM_Modbus
Pump Control Module over modbus

This program uses other open source software.

Some libraries have been modified to reduce size or add functionality to the sketch.

project is currently in pre-beta
currently only known to work on

Atmega328p and ATmega32u4.

credits: 

Arduino Stepper library

Arduino Servo library

ModbusRTUSlave library by C. M. Bulliner
2023-12-29
- Additional features added.
  - New Motor class model to simplify implementation and option to bypass default motor handler.
  - System uptime counter.
  - Available memory display has been improved
  - New timer implementation increased loop speed
- BugFix
  - Reboot when using servos fixed.
