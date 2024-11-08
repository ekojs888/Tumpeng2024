#include <Arduino.h>
// #include <WiFi.h>
#include "kelopak.h"

kelopak teratai;

// Wi-Fi configuration
const char *ssid = "dansat";
const char *password = "77040109";
// WiFiServer server(80);

void setup()
{
  Serial.begin(115200);
  teratai.begin();
  
  // // Start Wi-Fi as Access Point
  // WiFi.softAP(ssid, password);
  // server.begin();
  // Serial.println("WiFi started. Waiting for clients...");
  
  delay(3000);
  Serial.println("kelopak bunga v.2");
}

void loop()
{
  teratai.run();

  // check button push
  if (teratai.getBtnBuka())
  {
    Serial.println("tombol buka ....");
    teratai.setBuka();
    delay(500);
  }
  if (teratai.getBtnTutup())
  {
    Serial.println("tombol tutup ....");
    teratai.setTutup();
    delay(500);
  }

  // Check for client connections
  // WiFiClient client = server.available();
  // if (client)
  // {
  //   Serial.println("Client connected.");
  //   String request = client.readStringUntil('\r');
  //   Serial.println("Request: " + request);

  //   client.println(request);

  //   Serial.println(request.indexOf("BUKA"));
  //   // Command handling
  //   if (request.indexOf("BUKA") != -1 && !teratai.isRunning())
  //   {
  //     teratai.setBuka();
  //     client.println("Petals are opening...");
  //   }
  //   client.stop();
  //   Serial.println("Client disconnected.");
  // }
}
