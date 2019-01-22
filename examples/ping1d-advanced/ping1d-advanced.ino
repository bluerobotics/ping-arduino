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

#include "SoftwareSerial.h"

SoftwareSerial pingSerial = SoftwareSerial(9, 10);

static char _debug_buffer[200];
#define printf(fmt, args ...)  do {sprintf(_debug_buffer, fmt "\n\r", ## args); Serial.print(_debug_buffer);} while(0)

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
        printf("got processor temperature");
        printf("> processor_temperature: %d", ping.processor_temperature());
    } else {
        printf("attempt to get processor temperature failed");
    }

    // Get general info

    if (ping.request(Ping1DNamespace::General_info)) {
        printf("got general info");
        printf("> firmware_version_major: %d", ping.firmware_version_major());
        printf("> firmware_version_minor: %d", ping.firmware_version_minor());
        printf("> voltage_5: %d", ping.voltage_5());
        printf("> ping_interval: %d", ping.ping_interval());
        printf("> gain_index: %d", ping.gain_index());
        printf("> mode_auto: %d", ping.mode_auto());
    } else {
        printf("attempt to get general info failed");
    }

    if (ping.request(Ping1DNamespace::Profile)) {
        printf("got profile");
        printf("profile points: ");
        for (int i = 0; i < ping.profile_data_length(); i++) {
            printf("> %d", ping.profile_data()[i]);
        }
    } else {
        printf("attempt to get profile failed");
    }
}
