//#define DEBUG_MY_PROGRAM

#ifndef __MY_USEFUL_FUNCTIONS_H__
  #define __MY_USEFUL_FUNCTIONS_H__
  #define READBIT(A, B) ((A >> B) & 1)
  #define SETBIT(A, B, C) (A = C ? A | (1<<B) : A & ~(1<<B))

  #ifdef DEBUG_MY_PROGRAM
    #define d_print(a) Serial.print(__LINE__);Serial.print(" ");Serial.print(a);
    #define d_println(a) Serial.println(a);
  #else
    #define d_print(a)
    #define d_println(a)
  #endif

#endif