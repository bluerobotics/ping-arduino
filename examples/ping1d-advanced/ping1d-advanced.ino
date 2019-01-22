/**
 *  This example is targeted toward the arduino platform
 *
 *  This example demonstrates advanced usage of the Blue Robotics Ping1D
 *  c++ API
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

void toggleLed()
{
    digitalWrite(ledPin, !digitalRead(ledPin));
}

void setup()
{
    pingSerial.begin(19200);
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    Serial.println("Blue Robotics ping1d-advanced.ino");
    while (!ping.initialize()) {
        Serial.println("Ping device failed to initialize!");
        delay(2000);
    }
}

void loop()
{
    // Get processor temperature

    // If the result is true, we have received an update from the device
    // If the result is true, the accessors processor_temperature() etc.
    // will return the updated value
    if (ping.request(Ping1DNamespace::Processor_temperature)) {
        Serial.println("got processor temperature");
        Serial.print("> processor_temperature: ");
        Serial.println(ping.processor_temperature());
    } else {
        Serial.println("attempt to get processor temperature failed");
    }

    // Get general info

    if (ping.request(Ping1DNamespace::General_info)) {
        Serial.println("got general info");

        Serial.print("> firmware_version_major: ");
        Serial.println(ping.firmware_version_major());

        Serial.print("> firmware_version_minor: ");
        Serial.println(ping.firmware_version_minor());

        Serial.print("> voltage_5: ");
        Serial.println(ping.voltage_5());

        Serial.print("> ping_interval: ");
        Serial.println(ping.ping_interval());

        Serial.print("> gain_index: ");
        Serial.println(ping.gain_index());

        Serial.print("> mode_auto: ");
        Serial.println(ping.mode_auto());
    } else {
        Serial.println("attempt to get general info failed");
    }

    if (ping.request(Ping1DNamespace::Profile)) {
        Serial.println("got profile");
        Serial.println("profile points: ");
        for (int i = 0; i < ping.profile_data_length(); i++) {
            Serial.print(" > ");
            Serial.println(ping.profile_data()[i]);
        }
    } else {
        Serial.println("attempt to get profile failed");
    }
}
