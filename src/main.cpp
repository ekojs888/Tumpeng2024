#include <Arduino.h>

// Definisikan pin untuk driver stepper pada ESP32 atau Arduino Uno
#define NAIK true
#define TURUN false

#if defined(ESP32)
#define STEPPER_EN_PIN 23
#define STEPPER_DIR_PIN 5  // Pin arah yang digunakan bersama

#define STEPPER1_STEP_PIN 26
#define STEPPER2_STEP_PIN 25
#define STEPPER3_STEP_PIN 17
#define STEPPER4_STEP_PIN 16
#define STEPPER5_STEP_PIN 27
#define STEPPER6_STEP_PIN 14

#elif defined(ARDUINO_ARCH_AVR)
#define STEPPER_EN_PIN 2
#define STEPPER_DIR_PIN 3  // Pin arah yang digunakan bersama

#define STEPPER1_STEP_PIN 4
#define STEPPER2_STEP_PIN 5
#define STEPPER3_STEP_PIN 6
#define STEPPER4_STEP_PIN 7
#define STEPPER5_STEP_PIN 8
#define STEPPER6_STEP_PIN 9
#else
#error "Platform tidak didukung. Harap gunakan ESP32 atau Arduino Uno."
#endif

#define STEPS_PER_REVOLUTION 3200L // Langkah per putaran penuh

class StepperController {
private:
  int stepPin;
  long stepsRemaining;
  unsigned long lastStepTime;
  unsigned long stepInterval; // Waktu antar langkah dalam mikrodetik

public:
  StepperController(int stepPin)
      : stepPin(stepPin), stepsRemaining(0), lastStepTime(0), stepInterval(1000) {
    pinMode(stepPin, OUTPUT);
  }

  void setRPM(float rpm) {
    stepInterval = 60000000L / (STEPS_PER_REVOLUTION * rpm);
  }

  void move(long steps) {
    stepsRemaining = steps;
  }

  void update() {
    if (stepsRemaining > 0) {
      unsigned long currentTime = micros();
      if (currentTime - lastStepTime >= stepInterval) {
        lastStepTime = currentTime;
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(1);
        digitalWrite(stepPin, LOW);
        stepsRemaining--;
      }
    }
  }

  bool isMoving() {
    return stepsRemaining > 0;
  }
};

// Buat objek untuk setiap stepper
StepperController stepper1(STEPPER1_STEP_PIN);
StepperController stepper2(STEPPER2_STEP_PIN);
StepperController stepper3(STEPPER3_STEP_PIN);
StepperController stepper4(STEPPER4_STEP_PIN);

bool isOpening = true;

void setup() {
  pinMode(STEPPER_EN_PIN, OUTPUT);
  pinMode(STEPPER_DIR_PIN, OUTPUT);
  
  digitalWrite(STEPPER_EN_PIN, LOW); // Aktifkan driver motor
  digitalWrite(STEPPER_DIR_PIN, NAIK); // Set arah untuk semua motor

  // Set RPM untuk setiap motor
  stepper1.setRPM(350);
  stepper2.setRPM(350);
  stepper3.setRPM(350);
  stepper4.setRPM(350);

  // Set langkah untuk membuka kelopak
  long stepsToOpen[4] = {
    STEPS_PER_REVOLUTION * 140,
    STEPS_PER_REVOLUTION * 150,
    STEPS_PER_REVOLUTION * 150,
    STEPS_PER_REVOLUTION * 150,
  };

  // Mulai gerakan membuka
  stepper1.move(stepsToOpen[0]);
  stepper2.move(stepsToOpen[1]);
  stepper3.move(stepsToOpen[2]);
  stepper4.move(stepsToOpen[3]);
}

void loop() {
  stepper1.update();
  stepper2.update();
  stepper3.update();
  stepper4.update();

  if (isOpening &&
      !stepper1.isMoving() &&
      !stepper2.isMoving() &&
      !stepper3.isMoving() &&
      !stepper4.isMoving()) {
    isOpening = false;
    digitalWrite(STEPPER_EN_PIN, HIGH); // Nonaktifkan motor setelah selesai
  }
}
