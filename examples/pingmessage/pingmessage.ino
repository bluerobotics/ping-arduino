/**
 *  This example is targeted toward the arduino platform
 *
 *  This example demonstrates usage of the Blue Robotics PingMessage c++ API
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
// Here, we use pin 9 as arduino rx (Ping tx, white), 10 as arduino tx (Ping rx, green)
static const uint8_t arduinoRxPin = 9;
static const uint8_t arduinoTxPin = 10;
SoftwareSerial pingSerial = SoftwareSerial(arduinoRxPin, arduinoTxPin);
static Ping1D ping { pingSerial };

static PingParser parser;

static const uint8_t ledPin = 13;

bool waitMessage(enum Ping1DNamespace::msg_ping1D_id id, uint16_t timeout_ms = 400);

void setup()
{
    pingSerial.begin(9600);
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    Serial.println("Blue Robotics pingmessage.ino");
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
            Serial.print("> type: ");
            Serial.println(m.device_type());
            Serial.print("> model: ");
            Serial.println(m.device_model());
            Serial.print("> firmware version: ");
            Serial.print(m.firmware_version_major());
            Serial.print(".");
            Serial.println(m.firmware_version_minor());
            break;
        }

        case Ping1DNamespace::Voltage_5: {
            ping_msg_ping1D_voltage_5 m(parser.rxMsg);
            Serial.print("> device voltage: ");
            Serial.println(m.voltage_5());
            break;
        }

        case Ping1DNamespace::Processor_temperature: {
            ping_msg_ping1D_processor_temperature m(parser.rxMsg);
            Serial.print("> processor temperature: ");
            Serial.println(m.processor_temperature());
            break;
        }

        case Ping1DNamespace::General_info: {
            ping_msg_ping1D_general_info m(parser.rxMsg);
            Serial.print("> firmware version: ");
            Serial.print(m.firmware_version_major());
            Serial.print(".");
            Serial.println(m.firmware_version_minor());
            Serial.print("> device voltage: ");
            Serial.print(m.voltage_5());
            Serial.println("mV");
            Serial.print("> ping_interval: ");
            Serial.print(m.ping_interval());
            Serial.println("ms");
            Serial.print("> gain setting: ");
            Serial.println(gain_settings[m.gain_index()]);
            Serial.print("> Mode: ");
            Serial.println(m.mode_auto() ? "Auprintfto" : "Manual");
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

        Serial.print("> id: ");
        Serial.print(parser.rxMsg.message_id());
        Serial.print("\t Length: ");
        Serial.print(parser.rxMsg.payload_length());
        Serial.print("\t parsed: ");
        Serial.print(parser.parsed);
        Serial.print("\t errors: ");
        Serial.println(parser.errors);
    }

    // Toggle the LED to show that the program is running
    digitalWrite(ledPin, !digitalRead(ledPin));
}

bool waitMessage(enum Ping1DNamespace::msg_ping1D_id id, uint16_t timeout_ms = 400)
{
    uint32_t timeout_time = millis() + timeout_ms;
    while (millis() < timeout_time) {

        while(pingSerial.available()) {

            if (parser.parseByte(pingSerial.read()) == PingParser::NEW_MESSAGE) {
                if (parser.rxMsg.message_id() == id) {
                    return true;
                }
            }
        }
    }
    Serial.print("timeout waiting for message id: ");
    Serial.println(id);
    return false;
}
