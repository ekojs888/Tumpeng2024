#include "buzzer.h"
buzzer::buzzer(){}
buzzer::buzzer(int pin,int dly,int bzrOn){
    pinBuzzer = pin;
    delayBuzzer = dly;
    buzzerOn = bzrOn;
    pinMode(pinBuzzer, OUTPUT);
    digitalWrite(pinBuzzer, !buzzerOn); // Matikan buzzer
    buzEn = false; // Inisialisasi status awal buzzer
}
buzzer::buzzer(int pin, int dly)
{
    pinBuzzer = pin;
    delayBuzzer = dly;
    pinMode(pinBuzzer, OUTPUT);
    digitalWrite(pinBuzzer, HIGH); // Matikan buzzer
    buzEn = false; // Inisialisasi status awal buzzer
}

void buzzer::setBuzzer()
{
    digitalWrite(pinBuzzer, buzzerOn); // Nyalakan buzzer
    buzEn = true;                  // Set status buzzer aktif
    lastStepTime = millis();       // Simpan waktu awal saat buzzer diaktifkan
    Serial.printf("buser aktif %ld \r\n",lastStepTime);
}

void buzzer::run()
{
    if (buzEn) // Cek apakah buzzer sedang aktif
    {
        unsigned long currentTime = millis();
        if (currentTime - lastStepTime >= delayBuzzer) // Jika waktu aktif sudah tercapai
        {
            Serial.printf("buser nonaktif %ld \r\n",lastStepTime);
            digitalWrite(pinBuzzer, !buzzerOn); // Matikan buzzer
            buzEn = false;               // Reset status untuk menghindari pengulangan
        }
    }
}