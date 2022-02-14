#include "ping1d.h"

static const uint8_t arduinoRxPin = 19; //Serial1 rx
static const uint8_t arduinoTxPin = 18; //Serial1 tx

static Ping1D ping { Serial1 };

static const uint8_t ledPin = 13;

void setup()
{
  Serial1.begin(115200);
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  Serial.println("Blue Robotics ping1d-simple.ino");
  while (!ping.initialize()) {
    Serial.println("\nPing device failed to initialize!");
    Serial.println("Are the Ping rx/tx wired correctly?");
    Serial.print("Ping rx is the green wire, and should be connected to Arduino pin ");
    Serial.print(arduinoTxPin);
    Serial.println(" (Arduino tx)");
    Serial.print("Ping tx is the white wire, and should be connected to Arduino pin ");
    Serial.print(arduinoRxPin);
    Serial.println(" (Arduino rx)");
    delay(2000);
  }
}

void loop()
{
  if (ping.update()) {
    Serial.print("Distance: ");
    Serial.print(ping.distance());
    Serial.print("\tConfidence: ");
    Serial.println(ping.confidence());
  } else {
    Serial.println("No update received!");
  }

  // Toggle the LED to show that the program is running
  digitalWrite(ledPin, !digitalRead(ledPin));
}
