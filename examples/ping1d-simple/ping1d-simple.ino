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

#include "SoftwareSerial.h"

SoftwareSerial pingSerial = SoftwareSerial(9, 10);

#include "pingmessage_all.h"
#include "ping_parser.h"
#include "ping1d.h"

static Ping1D ping { pingSerial };

static const uint8_t ledPin = 13;

void toggleLed()
{
    digitalWrite(ledPin, !digitalRead(ledPin));
}

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
        toggleLed();
    } else {
        Serial.println("No update received!");
    }
}
