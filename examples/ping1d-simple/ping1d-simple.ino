/**
 *  This example is targeted toward the arduino platform
 *
 *  This example demonstrates the most simple usage of the Blue Robotics
 *  Ping1D c++ API in order to obtain distance and confidence reports from
 *  the device.
 *
 *  This API exposes the full functionality of the Ping1D Echosounder
 *
 *  Communication is performed with a Blue Robotics Ping1D Echosounder
 */

#include "ping1d.h"

// This serial port is used to communicate with the Ping device
// If you are using and Arduino UNO or Nano, you must use software serial and leave this line commented.
// If you are using a board with multiple hardware serial ports (such as the Mega 2560) you may use
// hardware serial and uncomment this line. This will allow faster communication.
//#define HARDWARE_SERIAL


#ifdef HARDWARE_SERIAL
  static const uint8_t arduinoRxPin = 19; // Pin 19: Arduino Serial1 RX, Ping TX, white wire
  static const uint8_t arduinoTxPin = 18; // Pin 18: Arduino Serial1 TX, Ping RX, green wire
  static const long baudRate = 115200;
  HardwareSerial& pingSerial = Serial1;
  static Ping1D ping { pingSerial };
#else
  #include "SoftwareSerial.h"
  static const uint8_t arduinoRxPin = 9;  // Pin 9: Arduino RX, Ping TX, white wire
  static const uint8_t arduinoTxPin = 10; // Pin 10: Arduino TX, Ping RX, green wire
  static const long baudRate = 9600;
  SoftwareSerial pingSerial = SoftwareSerial(arduinoRxPin, arduinoTxPin);
  static Ping1D ping { pingSerial };
#endif

static const uint8_t ledPin = 13;

void setup()
{
    pingSerial.begin(baudRate);
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    Serial.println("Blue Robotics ping1d-simple.ino");
    while (!ping.initialize()) {
        Serial.println("\nPing device failed to initialize!");
        Serial.println("Are the Ping RX/TX wired correctly?");
        Serial.print("Ping RX is the green wire, and should be connected to Arduino pin ");
        Serial.print(arduinoTxPin);
        Serial.println(" (Arduino TX)");
        Serial.print("Ping TX is the white wire, and should be connected to Arduino pin ");
        Serial.print(arduinoRxPin);
        Serial.println(" (Arduino RX)");
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
