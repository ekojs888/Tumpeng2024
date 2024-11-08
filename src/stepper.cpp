
#include "stepper.h"
stepper::stepper()
{
}
stepper::stepper(int stepPin)
{
    this->stepPin = stepPin;
    pinMode(stepPin, OUTPUT);
}

void stepper::setRampStep(long rs)
{
    this->rampSteps = rs;
}

void stepper::setRPM(float rpm)
{
    //kalau rpm 300 hasil adalah delay 1 ms per detakan
    // stepInterval = 60000000L / (STEPS_PER_REVOLUTION * rpm);
    float stepsPerSecond = (rpm * stepPerRev) / 60.0;
    // baseInterval = (1000.0 / stepsPerSecond); // Milidetik per langkah
    stepInterval = (1000000.0 / stepsPerSecond); // mcrodetik per langkah
    baseInterval = stepInterval;

    Serial.printf("rpm : %f, hasil step: %ld \r\n",rpm,stepInterval);
}

long stepper::getCurrentPosition()
{
    return currentPosition;
}

void stepper::update()
{
    if (stepsRemaining < totalSteps)
    {
        unsigned long currentTime = micros();
        if (currentTime - lastStepTime >= stepInterval)
        {
            lastStepTime = currentTime;
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(5);
            digitalWrite(stepPin, LOW);

            // Serial.println(stepsRemaining);

            
            if (stepsRemaining < rampSteps) // akslerasi
            {
                stepInterval = baseInterval * (rampSteps - stepsRemaining) / rampSteps;
            }
            else if (stepsRemaining > (totalSteps - rampSteps)) //Deselerasi
            {
                stepInterval = baseInterval * (stepsRemaining - (totalSteps - rampSteps)) / rampSteps;
            }else{
                stepInterval = baseInterval;
            }

            stepsRemaining++;
            currentPosition++; // Update the current position
        }
    }
}

bool stepper::isMoving()
{
    return stepsRemaining < totalSteps;
}

void stepper::adjustMovement(int steps)
{
    totalSteps = stepPerRev * steps;
    stepsRemaining = 0;
    currentPosition = 0;
    Serial.printf("stepPerRev: %d , stepTotal: %ld \r\n",stepPerRev,totalSteps);
}

void stepper::setStepPerRev(int steps){
    stepPerRev = steps;
}