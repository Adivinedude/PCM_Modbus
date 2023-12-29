#ifndef __MY_USEFUL_FUNCTIONS_H__
  #define __MY_USEFUL_FUNCTIONS_H__
  #define READBIT(A, B) ((A >> B) & 1)
  #define SETBIT(A, B, C) (A = C ? A | (1<<B) : A & ~(1<<B))

  //#define DEBUG_MY_PROGRAM

  #ifdef DEBUG_MY_PROGRAM
    #define d_print(a) Serial.print(__LINE__);Serial.print(" ");Serial.print(a);
    #define d_println(a) Serial.print(a); Serial.print(" "); Serial.println(millis());

    #define debug_triger_global bool debug_trigger = false;
    #define debug_triger_function \
    if(!debug_trigger&&(millis() > 10000)) \
    { \
      coil_data[0] = 1; \
      hr_data[0] = 180; \
      hr_data[1] = 180; \
      hr_data[2] = 180; \
      hr_data[3] = 1; \
    }

  #else
    #define d_print(a)
    #define d_println(a)
    #define debug_triger_global
    #define debug_triger_function
  #endif



#endif