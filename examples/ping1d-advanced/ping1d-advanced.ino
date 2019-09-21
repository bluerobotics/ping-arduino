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
    Serial.println("Blue Robotics ping1d-advanced.ino");
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

    // SoftwareSerial cannot handle long profile messages
    #ifdef HARDWARE_SERIAL
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
     #endif

    // Toggle the LED to show that the program is running
    digitalWrite(ledPin, !digitalRead(ledPin));
}
