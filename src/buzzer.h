#ifndef __BUZZER__
#define __BUZZER__
#include <Arduino.h>

class buzzer{
    private:
    int pinBuzzer;
    int delayBuzzer;
    bool buzEn;
    
    unsigned long lastStepTime;
    public:
    buzzer(int pin, int dly);
    void setBuzzer();
    void run();
};


#endif