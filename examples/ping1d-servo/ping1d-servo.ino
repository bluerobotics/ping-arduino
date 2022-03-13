/**
 *  This example is targeted toward the arduino platform
 *
 *  This example demonstrates basic usage of the Blue Robotics Ping1D c++ API
 *  together with a servo-controlled device.
 *
 *  Context for the example is in this forum thread:
 *  https://discuss.bluerobotics.com/t/using-ping1d-and-servo-with-arduino-nano-every/11715
 *
 *  Communication is performed with a Blue Robotics Ping1D Echosounder
 *  Includes servo-style control of a device (e.g. a servo motor)
 */

#include <Servo.h>
#include "ping1d.h"
#include "SoftwareSerial.h"

/* NOTE: Servo and SoftwareSerial clash!
 *   Both libraries use the same timer.
 *   This code avoids clashes by only keeping the servo 'attach'ed while moving.
 *   If the servo is under load, undesired movement may occur while using Ping.
 */

/* Ping Sonar */
void readSonar(bool display=true); // declare readSonar function
static const uint8_t arduinoRxPin = 9;
static const uint8_t arduinoTxPin = 10;
SoftwareSerial pingSerial = SoftwareSerial(arduinoRxPin, arduinoTxPin);
static Ping1D ping { pingSerial };
// speed of sound in operating medium (~343 m/s for air, ~1500 m/s for seawater)
static const uint32_t v_sound = 343000; // mm/s

/* Servo */
void moveServo(int angle, bool display=true); // declare moveServo function
Servo myservo;
// angles, in degrees
int pos = 0;
int max_angle;
int min_angle = 0;
int increment = 5;
static const uint8_t servoPin = 3;
// ms to allow for each rotation
//  can reduce multiplier to speed up, until not enough time to turn
uint32_t moveTime = 50 * abs(increment);


void setup() {
    // Initialise serial connections
    pingSerial.begin(9600);
    Serial.begin(115200);
    // Initialise Ping device (ensure correctly connected)
    while (!ping.initialize()){
        Serial.println("\nPing device failed to initialize!");
        Serial.print("green wire connects to: ");
        Serial.println(arduinoTxPin);
        Serial.print("white wire connects to: ");
        Serial.println(arduinoRxPin);
        delay(1000);
    }
    ping.set_speed_of_sound(v_sound);
    // Initialise servo position
    moveServo(min_angle, false);
}

void loop() {
    // Request user input for angle to rotate to
    Serial.println("Enter maximum angle value");
    // busy-wait indefinitely for a response
    while (!Serial.available()){}
    max_angle = Serial.readStringUntil('\n').toInt();
    // Response received - perform requested rotation (there and back)
    Serial.print("Rotating ");
    Serial.print(max_angle);
    Serial.println(" degrees");

    for (pos = min_angle; pos <= max_angle; pos+=increment){
        moveServo(pos);
        readSonar();
    }

    for (pos = max_angle; pos >= min_angle; pos-=increment){
    moveServo(pos);
    readSonar();
    }
}

/* Gets a sonar measurement, and displays the results. */
void readSonar() {
  // request measurements until one arrives
  while (!ping.update(bool display)){}
  // display the results
  Serial.print("Distance: ");
  Serial.print(ping.distance());
  Serial.print(" mm\tConfidence: ");
  Serial.print(ping.confidence());
  Serial.println("%");
}

/* Engages the servo while moving to the requested 'angle'.
 *  If 'display' is true, prints the current angle request.
 */
void moveServo(int angle, bool display) {
  myservo.attach(servoPin);
  myservo.write(angle);
  delay(moveTime);
  if (display) {
    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print("\t");
  }
  myservo.detach();
}
