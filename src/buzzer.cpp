#include "buzzer.h"

buzzer::buzzer(int pin, int dly)
{
    pinBuzzer = pin;
    delayBuzzer = dly;
    pinMode(pinBuzzer, OUTPUT);
    buzEn = false; // Inisialisasi status awal buzzer
}

void buzzer::setBuzzer()
{
    digitalWrite(pinBuzzer, HIGH); // Nyalakan buzzer
    buzEn = true;                  // Set status buzzer aktif
    lastStepTime = millis();       // Simpan waktu awal saat buzzer diaktifkan
}

void buzzer::run()
{
    if (buzEn) // Cek apakah buzzer sedang aktif
    {
        unsigned long currentTime = millis();
        if (currentTime - lastStepTime >= delayBuzzer) // Jika waktu aktif sudah tercapai
        {
            digitalWrite(pinBuzzer, LOW); // Matikan buzzer
            buzEn = false;               // Reset status untuk menghindari pengulangan
        }
    }
}