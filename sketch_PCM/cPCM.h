#ifndef CPCM_H
#define CPCM_H
    #include <stdint.h>
    class cMotor_Base_Class;
    class cPCM{
        public:    
            bool        *c,*d;
            uint16_t    *h,*i;
            uint8_t     pump_state;         // current state of the machine
            uint32_t    endcycle_feedback,  // Feedback counter value at end of current cycle
                        total_feedback,     // feedback counter when total amount has been dispenced
                        timer;              // time the current cycle ends.
            uint16_t    calibration,        // current command paramaters
                        duration,           // how long to dispence set amount
                        dose;               // how much to dispence each second
            volatile uint32_t* feedback_counter;
            uint8_t     id;
            cMotor_Base_Class* motor;
            
            static cMotor_Base_Class* GetNewMotor(uint8_t id);

            cPCM();
            void run();
    };
#endif