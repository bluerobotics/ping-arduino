#include <pingmessage_all.h>
#include <pingmessage.h>
#include <pingmessage_ping1D.h>
#include "parser_ping.h"

#include <SoftwareSerial.h>
SoftwareSerial pingSerial(10, 11);

PingParser p;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pingSerial.begin(115200);
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  ping_msg_ping1D_empty m;
  m.set_id(Ping1DNamespace::Distance);
  m.updateChecksum();
  pingSerial.write(m.msgData, m.msgDataLength());
  while(pingSerial.available()) {
    if(p.parseByte(pingSerial.read()) == PingParser::NEW_MESSAGE) {
      Serial.print(p.rxMsg.message_id());       Serial.print(' '); Serial.println(p.rxMsg.payload_length());
      //if (p.rxMsg.message_id() == Ping1DNameSpace

      //uint16_t a = p.rxMsg.message_id();
      //pingSerial.write((char*)&a, 2);
      
      digitalWrite(13, !digitalRead(13));
    }
  }
  delay(250);
}
