<a href="https://bluerobotics.com">
 <img src="https://avatars2.githubusercontent.com/u/7120633?v=3&s=100" align="left" hspace="10" vspace="6">
</a>

[![Travis Build Status](https://travis-ci.org/bluerobotics/ping-arduino.svg?branch=master)](https://travis-ci.org/bluerobotics/ping-arduino)
[![Gitter](https://img.shields.io/badge/gitter-online-green.svg)](https://gitter.im/bluerobotics/ardusub/)

The [Blue Robotics Ping Echosounder](https://www.bluerobotics.com/store/sensors-sonars-cameras/sonar/ping-r1/) is a low cost acoustic rangefinder for underwater applications. This Arduino library allows you to communicate and gather data from a Ping device.

## Ping Firmware Setup

[Change](https://github.com/bluerobotics/ping-viewer/wiki/firmware-update) the Ping device firmware to 9600 baud.

## Arduino Library Installation

This library can be found in the Arduino Library manager. To install the library, open the Arduino IDE and navigate to library manager with the menu items: *Sketch* -> *Include Library* -> *Manage Libraries*. Search for `Blue Robotics ping-arduino` to find the library, then click Install to install it.

![](https://www.arduino.cc/en/uploads/Guide/LibraryManager_1.png)

More information about installing Arduino libraries can be found [here](https://www.arduino.cc/en/guide/libraries).

## Connections

These are the recommended wiring connections to make between the Ping device and an Arduino:

| Ping | Arduino |
|------|---------|
| red (5V) | 5V |
| black (Ground) | Ground |
| white (tx) | pin 10 (SoftwareSerial rx) |
| green (rx) | pin 9 (SoftwareSerial tx) |

## Examples

You can begin using your Ping with an Arduino by uploading the [ping1d-simple](/examples/ping1d-simple/ping1d-simple.ino) example in the [examples](/examples) folder.

The built in HardwareSerial port is used to print out data from the ping device over the Arduino's usb port. To view the data, open the Arduino IDE Serial monitor, and set the baudrate to 115200.

The examples use the SoftwareSerial library to communicate with the Ping device, which allows flexibility for the rx/tx pin assignments on the Arduino. The examples use pins 10 and 9. The SoftwareSerial library [**can only communicate with the Ping device at 9600 baud**](#ping-firmware-setup). If you are using a board with more than one HardwareSerial port, like the Arduino MEGA, you may use the second serial port to communicate with a Ping device at 115200 baud.

## Usage

The Ping1D object is created with a reference to the Serial object that the Ping device is connected to.

```cpp
#include "SoftwareSerial.h"
#include "ping1d.h"
SoftwareSerial pingSerial { 9, 10 };
Ping1D myPing { pingSerial };
```

To begin communicating with the Ping device, first initialize the serial port with `begin(<baudrate>)`. Then, you can initialize the Ping device with [`initialize()`](http://docs.bluerobotics.com/ping-arduino/class_ping1_d.html#a103b04fc987383d0db256852ad12249b). `initialize()` will return false if the the device communication fails.

```cpp
void setup {
    // Use 9600 bits per second to communicate with the Ping device
    myPing.begin(9600);

    // Use built in Serial port to communicate with the Arduino IDE Serial Monitor
    Serial.begin(9600);

    // wait until communication with the Ping device is established
    // and the Ping device is successfully initialized
    while (!myPing.initialize()) {
        Serial.println("Ping device did not initialize");
    }

    Serial.println("Ping device initialized!)
}
```

### Basic

To get the [distance and confidence measurements](https://github.com/bluerobotics/ping-viewer/wiki#target-locking) from the Ping device, call [`update()`](http://docs.bluerobotics.com/ping-arduino/class_ping1_d.html#a57e42863075c4b0026bef3ef4418c116) to request a new distance measurement from the device before calling [`distance()`](http://docs.bluerobotics.com/ping-arduino/class_ping1_d.html#ab0b70fd7133b564a96f09cc7df9591ec) and [confidence()](http://docs.bluerobotics.com/ping-arduino/class_ping1_d.html#ae0f8635de222c8a095c42b1b1d3ad3f6) to use the measurement values. The values returned by the `distance()` and `confidence()` functions will not change until `update()` is called again.

```cpp
void loop() {
    while (!myPing.update()) {
        Serial.println("Ping device update failed");
    }

    Serial.println("Got an update!");
    Serial.print("distance: ");
    Serial.println(myPing.distance());
    Serial.print(" confidence: ");
    Serial.println(myPing.confidence());
}
```

### Advanced

The Ping communicates with the [Ping communication protocol](https://github.com/bluerobotics/ping-protocol).

Call [`request(<message id>)`](http://docs.bluerobotics.com/ping-arduino/class_ping1_d.html#a97f5121aee989b3d7048feb42367efcf) to request a message from the ping device. The message ids are also placed in the [`Ping1DNamespace`](http://docs.bluerobotics.com/ping-arduino/namespace_ping1_d_namespace.html) for convenience. To use the values from the response, use the Ping1D class accessors matching the message field names (eg. [`distance()`](http://docs.bluerobotics.com/ping-arduino/class_ping1_d.html#ab0b70fd7133b564a96f09cc7df9591ec) and [`speed_of_sound()`](http://docs.bluerobotics.com/ping-arduino/class_ping1_d.html#aa8c3bb77e49fb260558ac9e8276cd345)).

```cpp
void loop() {
    while (!myPing.request(Ping1DNamespace::Processor_temperature)) {
        Serial.println("Failed to get processor temperature");
    }
    Serial.println("Got an update!");
    Serial.print("processor temperature: ");
    Serial.println(myPing.processor_temperature());
}
```

See the [doxygen documentation](http://docs.bluerobotics.com/ping-arduino/index.html) for a full description of the available classes and functions.
