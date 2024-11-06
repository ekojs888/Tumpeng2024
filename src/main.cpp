#include <Arduino.h>
#include <WiFi.h>

#define NAIK LOW
#define TURUN HIGH

#define STEPPER_EN_PIN 18
#define STEPPER_DIR_PIN 19
#define STEPPER1_STEP_PIN 23
#define STEPPER2_STEP_PIN 5
#define STEPPER3_STEP_PIN 13

#define STEPS_PER_REVOLUTION 200L // Steps per full revolution
#define MAX_STEPS (STEPS_PER_REVOLUTION * 130)  // Maximum steps for full open/close (can be adjusted as needed)

class StepperController
{
private:
  int stepPin;
  long stepsRemaining;
  long currentPosition;  // Track the current position (in steps)
  unsigned long lastStepTime;
  unsigned long stepInterval;

public:
  StepperController(int stepPin) : stepPin(stepPin), stepsRemaining(0), currentPosition(0), lastStepTime(0), stepInterval(1000)
  {
    pinMode(stepPin, OUTPUT);
  }

  void setRPM(float rpm)
  {
    stepInterval = 60000000L / (STEPS_PER_REVOLUTION * rpm);
  }

  void move(long steps)
  {
    stepsRemaining = steps;
  }

  long getCurrentPosition()
  {
    return currentPosition;
  }

  void update()
  {
    if (stepsRemaining > 0)
    {
      unsigned long currentTime = micros();
      if (currentTime - lastStepTime >= stepInterval)
      {
        lastStepTime = currentTime;
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(1);
        digitalWrite(stepPin, LOW);
        stepsRemaining--;
        currentPosition++; // Update the current position
      }
    }
  }

  bool isMoving()
  {
    return stepsRemaining > 0;
  }

  void reset()
  {
    stepsRemaining = 0; // Stop the motor immediately, but do not reset currentPosition
  }

  void adjustMovement(long steps)
  {
    stepsRemaining = steps;
  }
};

// Create objects for each stepper motor
StepperController stepper1(STEPPER1_STEP_PIN);
StepperController stepper2(STEPPER2_STEP_PIN);
StepperController stepper3(STEPPER3_STEP_PIN);

bool isOpening = true;
bool isInterrupt = false;
bool isComplate = false;
// Wi-Fi configuration
const char *ssid = "sae";
const char *password = "77040109";
WiFiServer server(80);

void setup()
{
  Serial.begin(115200);

  // Configure pins
  pinMode(STEPPER_EN_PIN, OUTPUT);
  pinMode(STEPPER_DIR_PIN, OUTPUT);

  digitalWrite(STEPPER_EN_PIN, LOW); // Enable motor driver

  // Set RPM for each motor
  stepper1.setRPM(400);
  stepper2.setRPM(400);
  stepper3.setRPM(400);

  // Start Wi-Fi as Access Point
  WiFi.softAP(ssid, password);
  server.begin();

  Serial.println("WiFi started. Waiting for clients...");
}

void openPetals()
{
  digitalWrite(STEPPER_DIR_PIN, NAIK);
  stepper1.adjustMovement(MAX_STEPS - stepper1.getCurrentPosition());  // Adjust steps based on current position
  stepper2.adjustMovement(MAX_STEPS - stepper2.getCurrentPosition());
  stepper3.adjustMovement(MAX_STEPS - stepper3.getCurrentPosition());
  isOpening = true;
  isInterrupt = false;
  isComplate = false;
  Serial.println("Opening petals...");
  Serial.println(stepper1.getCurrentPosition());
}

void closePetals()
{
  digitalWrite(STEPPER_DIR_PIN, TURUN);
  stepper1.adjustMovement(MAX_STEPS - stepper1.getCurrentPosition());  // Adjust steps based on current position
  stepper2.adjustMovement(MAX_STEPS - stepper2.getCurrentPosition());
  stepper3.adjustMovement(MAX_STEPS - stepper3.getCurrentPosition());
  isOpening = false;
  isInterrupt = false;
  isComplate = false;
  Serial.println("Closing petals...");
  Serial.println(stepper1.getCurrentPosition());
}

void loop()
{
  // Update steppers
  if (!isInterrupt)
  {
    stepper1.update();
    stepper2.update();
    stepper3.update();
  }

  // Check if movement is complete
  if (!isComplate && !isInterrupt && !stepper1.isMoving() && !stepper2.isMoving() && !stepper3.isMoving())
  {
    digitalWrite(STEPPER_EN_PIN, HIGH); // Disable motor after movement
    Serial.println("Petal movement complete.");
    isComplate = true;
  }

  // Check for client connections
  WiFiClient client = server.available();
  if (client)
  {
    Serial.println("Client connected.");
    String request = client.readStringUntil('\r');
    Serial.println("Request: " + request);

    client.println(request);
    
    Serial.println(request.indexOf("BUKA"));
    // Command handling
    if (request.indexOf("BUKA") != -1)
    {
      if (!isOpening)
      { // If closing, interrupt and start opening
        stepper1.reset();
        stepper2.reset();
        stepper3.reset();
        openPetals();
      }
      client.println("Petals are opening...");
    }
    else if (request.indexOf("TUTUP") != -1)
    {
      if (isOpening)
      { // If opening, interrupt and start closing
        stepper1.reset();
        stepper2.reset();
        stepper3.reset();
        closePetals();
      }
      client.println("Petals are closing...");
    }
    else
    {
      client.println("Invalid command. Use 'BUKA' to open or 'TUTUP' to close.");
    }
    client.stop();
    Serial.println("Client disconnected.");
  }
}
