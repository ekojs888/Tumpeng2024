#ifndef __KELOPAK__
#define __KELOPAK__
#include <Arduino.h>
#include "stepper.h"
#include "buzzer.h"

#define KELOPAK_TUTUP_DIR LOW
#define KELOPAK_BUKA_DIR HIGH

#define KELOPAK_BTN_TUTUP 27
#define KELOPAK_BTN_BUKA 14
#define KELOPAK_BTN_TUTUP_ON LOW
#define KELOPAK_BTN_BUKA_ON LOW

#define KELOPAK_PIN_BUZZER 16
#define KELOPAK_BUZZER_ON LOW
#define KELOPAK_BUZZER_DELAY_ON 5000

#define KELOPAK_EN_PIN 18
#define KELOPAK_DIR_PIN 19

#define KELOPAK1_STEP_PIN 23
#define KELOPAK2_STEP_PIN 5
#define KELOPAK3_STEP_PIN 13

#define KELOPAK_NUM_REV 130
#define KELOPAK_SPEED 500

class kelopak
{
private:
    stepper stpr1, stpr2, stpr3;
    buzzer serine;

    bool posisiBuka;
    bool posisiTutup;

    bool runningStatusCheckd; // true if sudah pengecekan setatus;

    void setEnable(bool en);
    void setDir(int dr);
    bool getStepperRunning();
    void setMove();
    void setMove(int numRev);

public:
    kelopak();
    void begin();
    void run();
    bool getBtnBuka();
    bool getBtnTutup();
    bool isRunning();
    void setBuka();
    void setTutup();
};

#endif