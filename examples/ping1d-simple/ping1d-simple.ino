#include "SoftwareSerial.h"
static char _debug_buffer[200];

SoftwareSerial pingSerial = SoftwareSerial(9, 10);
HardwareSerial& debugSerial = Serial;

#define debug(fmt, args ...)  do {sprintf(_debug_buffer, "[%s:%d]: " fmt "\n\r", __FUNCTION__, __LINE__, ## args); debugSerial.print(_debug_buffer);} while(0)

#define printf(fmt, args ...)  do {sprintf(_debug_buffer, fmt "\n\r", ## args); debugSerial.print(_debug_buffer);} while(0)

#include "pingmessage_all.h"
#include "ping_parser.h"
#include "ping1d.h"

static PingParser p;

static Ping1D pd { pingSerial, 19200 };

void toggleLed() {
  digitalWrite(13, !digitalRead(13));
}

void setup() {
  pingSerial.begin(19200);
  debugSerial.begin(115200);
  pinMode(13, OUTPUT);
  debugSerial.println("Blue Robotics Ping1D Example");
  while (!pd.initialize()) {
    debugSerial.println("Ping device failed to initialize!");
    delay(2000);
  }
}

void loop() {
  while (1) {
    if (pd.update()) {
      printf("Distance: %d\tConfidence: %d", pd.distance(), pd.confidence());
      toggleLed();
    } else {
      printf("No update received!");
    }
  }
}

