#include <Arduino.h>
#include <WiFi.h>
#include "stepper.h"
#include "buzzer.h"

#define NAIK LOW
#define TURUN HIGH

#define BUTTON_UP 27
#define BUTTON_DOWN 14

#define PIN_BUZZER 16

#define STEPPER_EN_PIN 18
#define STEPPER_DIR_PIN 19
#define STEPPER_NUM_STEP 3
#define STEPPER1_STEP_PIN 23
#define STEPPER2_STEP_PIN 5
#define STEPPER3_STEP_PIN 13

#define JUMLAH_PUTARAN 130
#define STEPPER_RPM 360

bool buttonDownPushd, buttonUpPushd;
bool sedangTurun, sedangNaik;

//buzzer
buzzer serine(PIN_BUZZER,5000);

stepper stepper1(STEPPER1_STEP_PIN);
stepper stepper2(STEPPER2_STEP_PIN);
stepper stepper3(STEPPER3_STEP_PIN);

void kelopakENABLE(bool en)
{
  if (en)
  {
    digitalWrite(STEPPER_EN_PIN, LOW); // enable
  }
  else
  {
    digitalWrite(STEPPER_EN_PIN, HIGH); // disable
  }
}
void kelopaDIR(int dir)
{
  digitalWrite(STEPPER_DIR_PIN, dir);
}

void kelopakMove()
{
  stepper1.adjustMovement(JUMLAH_PUTARAN);
  stepper2.adjustMovement(JUMLAH_PUTARAN);
  stepper3.adjustMovement(JUMLAH_PUTARAN);

  Serial.printf("step sekarang : %d \r\n", JUMLAH_PUTARAN);
}

void kelopakBUKA()
{
  if (!sedangTurun)
  {
    Serial.println("kelopak membuka.....");

    //aktifkan buzzer
    serine.setBuzzer();

    kelopakENABLE(true);
    kelopaDIR(TURUN);

    kelopakMove();

    sedangTurun = true;
    sedangNaik = false;
  }
  else
  {
    Serial.println("kelopak sedang terbuka.....");
  }

  buttonDownPushd = false;
}
void kelopakTUTUP()
{
  if (!sedangNaik)
  {
    Serial.println("kelopak mentutup.....");
    kelopakENABLE(true);
    kelopaDIR(NAIK);

    kelopakMove();
    sedangTurun = false;
    sedangNaik = true;
  }
  else
  {
    Serial.println("kelopak sedang mentutup.....");
  }

  buttonUpPushd = false;
}
void kelopakRPM()
{
  stepper1.setRPM(STEPPER_RPM);
  stepper2.setRPM(STEPPER_RPM);
  stepper3.setRPM(STEPPER_RPM);
}

void kelopakBegin()
{
  pinMode(STEPPER_EN_PIN, OUTPUT);
  pinMode(STEPPER_DIR_PIN, OUTPUT);

  kelopakRPM();
}

// Wi-Fi configuration
const char *ssid = "dansat";
const char *password = "77040109";
WiFiServer server(80);




void setup()
{
  Serial.begin(115200);

  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);

  digitalWrite(BUTTON_UP, HIGH);
  digitalWrite(BUTTON_DOWN, HIGH);

  kelopakBegin();

  // // Start Wi-Fi as Access Point
  WiFi.softAP(ssid, password);
  server.begin();
  Serial.println("WiFi started. Waiting for clients...");

  delay(3000);
  Serial.println("kelopak bunga v.1");

  sedangNaik = false;
  sedangTurun = false;

}

void loop()
{
  serine.run();

  stepper1.update();
  stepper2.update();
  stepper3.update();

  // check button push
  if (digitalRead(BUTTON_DOWN) == LOW && !stepper1.isMoving()&& !stepper2.isMoving()&& !stepper3.isMoving())
  {
    Serial.println("tombol buka ....");

    kelopakBUKA();
    buttonDownPushd = true;
    delay(500);
  }
  if (digitalRead(BUTTON_UP) == LOW && !stepper1.isMoving()&& !stepper2.isMoving()&& !stepper3.isMoving())
  {
    Serial.println("tombol tutup ....");
    kelopakTUTUP();
    buttonUpPushd = true;

    delay(500);
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
    if (request.indexOf("BUKA") != -1 && !stepper1.isMoving()&& !stepper2.isMoving()&& !stepper3.isMoving())
    {
      kelopakBUKA();
      client.println("Petals are opening...");
    }
    // else if (request.indexOf("TUTUP") != -1)
    // {

    //   client.println("Petals are closing...");
    // }
    else
    {
      client.println("Invalid command. Use 'BUKA' to open or 'TUTUP' to close.");
    }
    client.stop();
    Serial.println("Client disconnected.");
  }

  
}
