#ifndef __STEPPER__
#define __STEPPER__

#include <Arduino.h>





// Steps per full revolution
#define STEPS_PER_REVOLUTION 200L
// Maximum steps for full open/close (can be adjusted as needed)
#define MAX_STEPS (STEPS_PER_REVOLUTION * 1)

class stepper
{
private:
    int stepPin;
    long stepsRemaining=0;
    long currentPosition =0; // Track the current position (in steps)
    unsigned long lastStepTime;
    unsigned long stepInterval;

public:
    stepper(int stepPin);
    void setRPM(float rpm);
    long getCurrentPosition();
    void update();
    bool isMoving();
    void adjustMovement(int steps);
};


// class kelopak
// {
// private:
//     int pinEn;
//     int pinDir;
//     int speed;
//     stepper klp[];
//     bool isOpening = true;
//     bool isInterrupt = false;
//     bool isComplate = false;

// public:
//     kelopak(int enPin,int dirPin);
//     void begin();
//     void enable(bool en);
//     void direction(int ud);
//     void openPetals();
//     void closePetals();
//     void setRPM(int rpm);
//     void update();
// };


#endif