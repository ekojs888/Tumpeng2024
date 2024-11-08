#ifndef __BUZZER__
#define __BUZZER__
#include <Arduino.h>

class buzzer{
    private:
    int pinBuzzer;
    int delayBuzzer;
    bool buzEn;
    int buzzerOn; //HIGH or LOW
    
    unsigned long lastStepTime;
    public:
    buzzer();
    buzzer(int pin, int dly);
    buzzer(int pin,int dly, int bzOn);  // pin,delay,bzeron HIGH or LOW
    void setBuzzer();
    void run();
};


#endif