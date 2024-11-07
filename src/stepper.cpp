
#include "stepper.h"

stepper::stepper(int stepPin)
{
    this->stepPin = stepPin;
    pinMode(stepPin, OUTPUT);
}

void stepper::setRPM(float rpm)
{
    // stepInterval = 60000000L / (STEPS_PER_REVOLUTION * rpm);
    float stepsPerSecond = (rpm * STEPS_PER_REVOLUTION) / 60.0;
    stepInterval = (1000.0 / stepsPerSecond); // Milidetik per langkah
}

long stepper::getCurrentPosition()
{
    return currentPosition;
}

void stepper::update()
{
    if (stepsRemaining > 0)
    {
        unsigned long currentTime = millis();
        if (currentTime - lastStepTime >= stepInterval)
        {
            lastStepTime = currentTime;
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(stepPin, LOW);

            Serial.println(stepsRemaining);
            
            stepsRemaining--;
            currentPosition++; // Update the current position
            
        }
    }
}

bool stepper::isMoving()
{
    return stepsRemaining > 0;
}

void stepper::adjustMovement(int steps)
{
    stepsRemaining = STEPS_PER_REVOLUTION * steps;
    currentPosition = 0;
}
