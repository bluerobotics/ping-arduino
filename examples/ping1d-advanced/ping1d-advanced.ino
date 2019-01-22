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
HardwareSerial& debugSerial = Serial;

static char _debug_buffer[200];
#define printf(fmt, args ...)  do {sprintf(_debug_buffer, fmt "\n\r", ## args); debugSerial.print(_debug_buffer);} while(0)

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
    debugSerial.begin(115200);
    pinMode(ledPin, OUTPUT);
    debugSerial.println("Blue Robotics ping1d-advanced.ino");
    while (!ping.initialize()) {
        debugSerial.println("Ping device failed to initialize!");
        delay(2000);
    }
}

void loop()
{
    // Get processor temperature

    // If the result is true, we have received an update from the device
    // If the result is true, the accessors processor_temperature() etc.
    // will return the updated value
    if (ping.get_processor_temperature()) {
        printf("got processor temperature");
        printf("> %d", ping.processor_temperature());
    } else {
        printf("attempt to get processor temperature failed");
    }

    // Get general info

    // If you want to hold on to results of each request locally, you may pass
    // pointer references cooresponding to the payload fields.
    // If we receive an update from the device, non-null arguments will be
    // populated with the updated values
    uint16_t version_major, version_minor, mvolts, msec_per_ping;
    uint8_t gain_index, is_auto;

    if (ping.get_general_info(
                &version_major,
                &version_minor,
                &mvolts,
                &msec_per_ping,
                &gain_index,
                &is_auto)) {
        printf("got general info");
        printf("> version_major: %d", version_major);
        printf("> version_minor: %d", version_minor);
        printf("> mvolts: %d", mvolts);
        printf("> msec_per_ping: %d", msec_per_ping);
        printf("> gain_index: %d", gain_index);
        printf("> is_auto: %d", is_auto);
    } else {
        printf("attempt to get general info failed");
    }

    if (ping.get_profile()) {
        printf("got profile");
        printf("profile points: ");
        for (int i = 0; i < ping.profile_data_length(); i++) {
            printf("> %d", ping.profile_data()[i]);
        }
    } else {
        printf("attempt to get profile failed");
    }
}
