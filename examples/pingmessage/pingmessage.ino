/**
 *  This example is targeted toward the arduino platform
 *
 *  This example demonstrates usage of the Blue Robotics PingMessage c++ API
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
static PingParser parser;

static const uint8_t ledPin = 13;

void toggleLed()
{
    digitalWrite(ledPin, !digitalRead(ledPin));
}

bool waitMessage(enum Ping1DNamespace::msg_ping1D_id id, uint16_t timeout_ms = 400)
{
    uint32_t tstart = millis();
    while (millis() < tstart + timeout_ms) {

        while(pingSerial.available()) {

            if (parser.parseByte(pingSerial.read()) == PingParser::NEW_MESSAGE) {
                if (parser.rxMsg.message_id() == id) {
                    return true;
                }
            }
        }
    }
    printf("timeout waiting for message id: %d", id);
    return false;
}

void setup()
{
    pingSerial.begin(19200);
    debugSerial.begin(115200);
    pinMode(ledPin, OUTPUT);
    debugSerial.println("Blue Robotics pingmessage.ino");
    while (!ping.initialize()) {
        debugSerial.println("Ping device failed to initialize!");
        delay(2000);
    }
}

void loop()
{

    static uint8_t counter = 0;

    static Ping1DNamespace::msg_ping1D_id requestIds[] = {
        Ping1DNamespace::Firmware_version,
        Ping1DNamespace::Voltage_5,
        Ping1DNamespace::Processor_temperature,
        Ping1DNamespace::General_info,
        Ping1DNamespace::Profile
    };

    static float gain_settings[] = { 0.6, 1.8, 5.5, 12.9, 30.2, 66.1, 144 };

    static int requestIdsSize = sizeof(requestIds)/sizeof(requestIds[0]);
    counter++;
    counter = counter % requestIdsSize; // move to the next request


    // An empty message is used for the request
    ping_msg_ping1D_empty m;

    // Set the request id
    m.set_id(requestIds[counter]);

    // Prepare the buffer and write to device
    m.updateChecksum();
    pingSerial.write(m.msgData, m.msgDataLength());

    // Wait for the response from the device
    if(waitMessage(requestIds[counter])) {

        // Handle the message
        switch(parser.rxMsg.message_id()) {

        case Ping1DNamespace::Firmware_version: {
            ping_msg_ping1D_firmware_version m(parser.rxMsg);
            printf("> type: %d", m.device_type());
            printf("> model: %d", m.device_model());
            printf("> firmware version: %d.%d", m.firmware_version_major(), m.firmware_version_minor());
            break;
        }

        case Ping1DNamespace::Voltage_5: {
            ping_msg_ping1D_voltage_5 m(parser.rxMsg);
            printf("> device voltage: %d", m.voltage_5());
            break;
        }

        case Ping1DNamespace::Processor_temperature: {
            ping_msg_ping1D_processor_temperature m(parser.rxMsg);
            printf("> processor temperature: %d", m.processor_temperature());
            break;
        }

        case Ping1DNamespace::General_info: {
            ping_msg_ping1D_general_info m(parser.rxMsg);
            printf("> firmware version: %d.%d", m.firmware_version_major(), m.firmware_version_minor());
            printf("> device voltage: %dmV", m.voltage_5());
            printf("> ping_interval: %dms", m.ping_interval());
            printf("> gain setting: %.1f", gain_settings[m.gain_index()]);
            printf("> Mode: %s", m.mode_auto() ? "Auto" : "Manual");
            break;
        }

        /* TODO
        case Ping1DNamespace::Profile: {
            ping_msg_ping1D_profile m(p.rxMsg);
            printf("> distance: %d", m.distance());
            printf("> confidence: %d", m.confidence());
            printf("> pulse_usec: %d", m.pulse_usec());
            printf("> ping_number: %d", m.ping_number());
            printf("> start_mm: %d", m.scan_start());
            printf("> length_mm: %d", m.scan_length());
            printf("> gain_index: %d", m.gain_index());
            printf("> num_points: %d", m.num_points());
            printf("> gain_index: %d", m.gain_index());
            for(int i =0; i < m.num_points(); i++) {
            printf("> data: %d", m.data()[i]);
            }
            break;
        }
        */

        default:

            break;
        }

        toggleLed();
        printf("> id: %d\t Length: %d\t parsed: %d\t errors: %d", parser.rxMsg.message_id(), parser.rxMsg.payload_length(),
              parser.parsed, parser.errors);
    }
}
