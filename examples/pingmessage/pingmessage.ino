/**
 *  This example is targeted toward the arduino platform
 *
 *  This example demonstrates usage of the Blue Robotics ping_message c++ API
 *
 *  Communication is performed with a Blue Robotics Ping1D Echosounder
 */

#include "ping-message-all.h"
#include "ping-parser.h"

#include "SoftwareSerial.h"

// This serial port is used to communicate with the Ping device
// If you are using and Arduino UNO or Nano, this must be software serial, and you must use
// 9600 baud communication
// Here, we use pin 9 as arduino rx (Ping tx, white), 10 as arduino tx (Ping rx, green)
static const uint8_t arduinoRxPin = 9;
static const uint8_t arduinoTxPin = 10;
SoftwareSerial pingSerial = SoftwareSerial(arduinoRxPin, arduinoTxPin);

static PingParser parser;

static const uint8_t ledPin = 13;

bool waitMessage(enum PingMessageId id, uint16_t timeout_ms = 400);

void setup()
{
    pingSerial.begin(9600);
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);

    // Loop until we get initial communications from the device
    do {
        Serial.println("Blue Robotics pingmessage.ino");

        // An empty message is used for the request
        ping_message m;

        // Request a firmware_version message
        m.set_message_id(static_cast<uint16_t>(PingMessageId::PING1D_FIRMWARE_VERSION));

        // Prepare the buffer and write to device
        m.updateChecksum();
        pingSerial.write(m.msgData, m.msgDataLength());
        delay(2000);
    } while(!waitMessage(PingMessageId::PING1D_FIRMWARE_VERSION)); // wait for the response
}

void loop()
{

    static uint8_t counter = 0;

    static PingMessageId requestIds[] = {
        PingMessageId::PING1D_FIRMWARE_VERSION,
        PingMessageId::PING1D_VOLTAGE_5,
        PingMessageId::PING1D_PROCESSOR_TEMPERATURE,
        PingMessageId::PING1D_GENERAL_INFO,
        // SoftwareSerial cannot handle the long profile messages
        // Uncomment to activate (HardwareSerial recommended)
        // PingMessageId::PING1D_PROFILE
    };

    static float gain_settings[] = { 0.6, 1.8, 5.5, 12.9, 30.2, 66.1, 144 };

    static int requestIdsSize = sizeof(requestIds)/sizeof(requestIds[0]);
    counter++;
    counter = counter % requestIdsSize; // move to the next request


    // An empty message is used for the request
    ping_message m;

    // Set the request id
    m.set_message_id(static_cast<uint16_t>(requestIds[counter]));

    // Prepare the buffer and write to device
    m.updateChecksum();
    pingSerial.write(m.msgData, m.msgDataLength());

    // Wait for the response from the device
    if(waitMessage(requestIds[counter])) {

        // Handle the message
        switch(static_cast<PingMessageId>(parser.rxMessage.message_id())) {

        case PingMessageId::PING1D_FIRMWARE_VERSION: {
            ping1d_firmware_version m(parser.rxMessage);
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

        case PingMessageId::PING1D_VOLTAGE_5: {
            ping1d_voltage_5 m(parser.rxMessage);
            Serial.print("> device voltage: ");
            Serial.println(m.voltage_5());
            break;
        }

        case PingMessageId::PING1D_PROCESSOR_TEMPERATURE: {
            ping1d_processor_temperature m(parser.rxMessage);
            Serial.print("> processor temperature: ");
            Serial.println(m.processor_temperature());
            break;
        }

        case PingMessageId::PING1D_GENERAL_INFO: {
            ping1d_general_info m(parser.rxMessage);
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
            Serial.println(gain_settings[m.gain_setting()]);
            Serial.print("> Mode: ");
            Serial.println(m.mode_auto() ? "Auto" : "Manual");
            break;
        }

        case PingMessageId::PING1D_PROFILE: {
            ping1d_profile m(parser.rxMessage);
            Serial.print("> distance: ");
            Serial.println(m.distance());
            Serial.print("> confidence: ");
            Serial.println(m.confidence());
            Serial.print("> transmit_duration: ");
            Serial.println(m.transmit_duration());
            Serial.print("> ping_number: ");
            Serial.println(m.ping_number());
            Serial.print("> start_mm: ");
            Serial.println(m.scan_start());
            Serial.print("> length_mm: ");
            Serial.println(m.scan_length());
            Serial.print("> gain_index: ");
            Serial.println(m.gain_setting());
            Serial.print("> profile_data_length: ");
            Serial.println(m.profile_data_length());
            Serial.print("> gain_index: ");
            Serial.println(m.gain_setting());
            for(int i =0; i < m.profile_data_length(); i++) {
                Serial.print("> ");
                Serial.println(m.profile_data()[i]);
            }
            break;
        }

        default:

            break;
        }

        Serial.print("> id: ");
        Serial.print(parser.rxMessage.message_id());
        Serial.print("\t Length: ");
        Serial.print(parser.rxMessage.payload_length());
        Serial.print("\t parsed: ");
        Serial.print(parser.parsed);
        Serial.print("\t errors: ");
        Serial.println(parser.errors);
    }

    // Toggle the LED to show that the program is running
    digitalWrite(ledPin, !digitalRead(ledPin));
}

bool waitMessage(enum PingMessageId id, uint16_t timeout_ms = 400)
{
    uint32_t timeout_time = millis() + timeout_ms;
    while (millis() < timeout_time) {

        while(pingSerial.available()) {

            if (parser.parseByte(pingSerial.read()) == PingParser::State::NEW_MESSAGE) {
                if (parser.rxMessage.message_id() == static_cast<uint16_t>(id)) {
                    return true;
                }
            }
        }
    }
    Serial.print("timeout waiting for message id: ");
    Serial.println(static_cast<uint16_t>(id));
    return false;
}
