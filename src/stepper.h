#ifndef __STEPPER__
#define __STEPPER__

#include <Arduino.h>

class stepper
{
private:
    int stepPin;
    long stepsRemaining = 0;
    long totalSteps;
    long currentPosition = 0; // Track the current position (in steps)
    unsigned long lastStepTime;
    unsigned long stepInterval;
    unsigned long baseInterval;
    unsigned long rampSteps = 15;

    int stepPerRev = 200;
    

public:
    stepper();
    stepper(int stepPin);
    void setRampStep(long rs);
    void setRPM(float rpm);
    long getCurrentPosition();
    void update();
    bool isMoving();
    void adjustMovement(int steps);
    void setStepPerRev(int steps);
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