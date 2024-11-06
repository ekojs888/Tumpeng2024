#include <Arduino.h>

#define NAIK LOW
#define TURUN HIGH

#define STEPPER_EN_PIN 18
#define STEPPER_DIR_PIN 19
#define STEPPER1_STEP_PIN 23
#define STEPPER2_STEP_PIN 5

#define STEPS_PER_REVOLUTION 200L // Langkah per putaran penuh

class StepperController {
private:
  int stepPin;
  long stepsRemaining;
  unsigned long lastStepTime;
  unsigned long stepInterval; // Waktu antar langkah dalam mikrodetik
  bool mulai;
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
  long getMove(){
    return stepsRemaining;
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

bool isOpening = true;

void setup() {
  pinMode(STEPPER_EN_PIN, OUTPUT);
  pinMode(STEPPER_DIR_PIN, OUTPUT);
  
  digitalWrite(STEPPER_EN_PIN, LOW); // Aktifkan driver motor
  digitalWrite(STEPPER_DIR_PIN, NAIK); // Set arah untuk semua motor

  // Set RPM untuk setiap motor
  stepper1.setRPM(400);
  stepper2.setRPM(400);

  // Set langkah untuk membuka kelopak
  long stepsToOpen[4] = {
    STEPS_PER_REVOLUTION * 130,
    STEPS_PER_REVOLUTION * 130,
    STEPS_PER_REVOLUTION * 130,
    STEPS_PER_REVOLUTION * 130,
  };

  // Mulai gerakan membuka
  stepper1.move(stepsToOpen[0]);
  stepper2.move(stepsToOpen[1]);
}

void loop() {

  stepper1.update();
  
  if(stepper1.getMove() > STEPS_PER_REVOLUTION * 30){
    stepper2.update();
  }
  
  if (isOpening &&
      !stepper1.isMoving() ) {
    isOpening = false;
    digitalWrite(STEPPER_EN_PIN, HIGH); // Nonaktifkan motor setelah selesai
  }
}
