#include "kelopak.h"

kelopak::kelopak()
{
    begin();
}
void kelopak::begin()
{
    pinMode(KELOPAK_EN_PIN, OUTPUT);
    pinMode(KELOPAK_DIR_PIN, OUTPUT);
    setEnable(false); // disable kelopak


#if (KELOPAK_BTN_TUTUP_ON == LOW)
    pinMode(KELOPAK_BTN_TUTUP, INPUT_PULLUP);
#else
    pinMode(KELOPAK_BTN_TUTUP, INPUT_PULLDOWN);
#endif

#if (KELOPAK_BTN_BUKA_ON == LOW)
    pinMode(KELOPAK_BTN_BUKA, INPUT_PULLUP);
#else
    pinMode(KELOPAK_BTN_BUKA_ON, INPUT_PULLDOWN);
#endif

    serine = buzzer(KELOPAK_PIN_BUZZER, KELOPAK_BUZZER_DELAY_ON, KELOPAK_BUZZER_ON);

    stpr1 = stepper(KELOPAK1_STEP_PIN);
    stpr2 = stepper(KELOPAK2_STEP_PIN);
    stpr3 = stepper(KELOPAK3_STEP_PIN);

    stpr1.setRPM(KELOPAK_SPEED);
    stpr2.setRPM(KELOPAK_SPEED);
    stpr3.setRPM(KELOPAK_SPEED);

    //set akslerasi
    stpr1.setRampStep(10);
    stpr2.setRampStep(10);
    stpr3.setRampStep(10);

    runningStatusCheckd = true; // set true agar saat awal tidak muncul ket kelopak berhenti
}
void kelopak::setEnable(bool en)
{
    digitalWrite(KELOPAK_EN_PIN, en ? LOW : HIGH);
}

void kelopak::setDir(int dr)
{
    digitalWrite(KELOPAK_DIR_PIN, dr);
}

void kelopak::run()
{
    serine.run();

    stpr1.update();
    stpr2.update();
    stpr3.update();

    // cek setatus kelopak telah selesai bergerak
    if (!getStepperRunning() && !runningStatusCheckd)
    {
        Serial.println("kelopak berhenti....");
        runningStatusCheckd = true;
    }
}

bool kelopak::getStepperRunning()
{
    return (stpr1.isMoving() && stpr2.isMoving() && stpr3.isMoving());
}
bool kelopak::isRunning()
{
    return getStepperRunning();
}
bool kelopak::getBtnBuka()
{
    return (digitalRead(KELOPAK_BTN_BUKA) == KELOPAK_BTN_BUKA_ON && !getStepperRunning());
}

bool kelopak::getBtnTutup()
{
    return (digitalRead(KELOPAK_BTN_TUTUP) == KELOPAK_BTN_TUTUP_ON && !getStepperRunning());
}

void kelopak::setMove()
{
    // pengecekan runing setatus di false
    runningStatusCheckd = false;

    stpr1.setRPM(KELOPAK_SPEED);
    stpr2.setRPM(KELOPAK_SPEED);
    stpr3.setRPM(KELOPAK_SPEED);

    stpr1.adjustMovement(KELOPAK_NUM_REV);
    stpr2.adjustMovement(KELOPAK_NUM_REV);
    stpr3.adjustMovement(KELOPAK_NUM_REV);

    Serial.printf("set step : %d \r\n", KELOPAK_NUM_REV);
}

void kelopak::setMove(int numRev)
{
    stpr1.adjustMovement(numRev);
    stpr2.adjustMovement(numRev);
    stpr3.adjustMovement(numRev);

    Serial.printf("set step : %d \r\n", numRev);
}

void kelopak::setBuka()
{
    if (!posisiBuka)
    {
        serine.setBuzzer();
        setEnable(true);
        setDir(KELOPAK_BUKA_DIR);
        setMove();

        posisiBuka = true;
        posisiTutup = false;

        Serial.println("kelopak membuka....");
    }
    else
    {
        Serial.println("warning!, posisi sudah membuka");
    }
}

void kelopak::setTutup()
{
    if (!posisiTutup)
    {
        serine.setBuzzer();
        setEnable(true);
        setDir(KELOPAK_TUTUP_DIR);
        setMove();

        posisiBuka = false;
        posisiTutup = true;
    }
    else
    {
        Serial.println("warning!, posisi sudah menutup");
    }
}