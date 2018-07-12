#include <pingmessage_all.h>
#include <pingmessage.h>
#include <pingmessage_ping1D.h>
#include "parser_ping.h"

//#include <SoftwareSerial.h>
//SoftwareSerial pingSerial(10, 11);

PingParser p;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial3.begin(115200);
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  ping_msg_ping1D_empty m;
  m.set_id(Ping1DNamespace::Voltage_5);
  m.updateChecksum();
  Serial.write(m.msgData, m.msgDataLength());
    delay(250);

  Serial3.println("hey");
  while(Serial.available()) {
    if(p.parseByte(Serial.read()) == PingParser::NEW_MESSAGE) {
      Serial3.print(p.rxMsg.message_id());       Serial3.print(' '); Serial.print(p.rxMsg.payload_length()); Serial3.print(' '); Serial3.print(p.parsed); Serial3.print(' '); Serial3.println(p.errors);
      if (p.rxMsg.message_id() == Ping1DNamespace::Pcb_temperature) {
        ping_msg_ping1D_pcb_temperature m(p.rxMsg);
        Serial3.print("Pcb temp: "); Serial3.println(m.temp());
      }

      //uint16_t a = p.rxMsg.message_id();
      //pingSerial.write((char*)&a, 2);
      
      digitalWrite(13, !digitalRead(13));
    }
  }
  Serial3.println("bye");
  while(1);
}
