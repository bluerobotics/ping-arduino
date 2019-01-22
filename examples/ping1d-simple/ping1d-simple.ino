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

#include "pingmessage_all.h"
#include "ping_parser.h"
#include "ping1d.h"

#include "SoftwareSerial.h"

// This serial port is used to communicate with the Ping device
// If you are using and Arduino UNO or Nano, this must be software serial, and you must use
// 9600 baud communication
SoftwareSerial pingSerial = SoftwareSerial(9, 10);
static Ping1D ping { pingSerial };

static const uint8_t ledPin = 13;

void setup()
{
    pingSerial.begin(19200);
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    Serial.println("Blue Robotics ping1d-simple.ino");
    while (!ping.initialize()) {
        Serial.println("Ping device failed to initialize!");
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
