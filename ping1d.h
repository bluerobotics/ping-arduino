#pragma once
#include "SoftwareSerial.h"
#include <pingmessage_all.h>


class Ping1D
{
public:
  Ping1D(SoftwareSerial& ser, uint32_t baudrate) : _serial ( ser )
  {
    _serial.begin(baudrate);
  }
  
  PingMessage* read()
  {
      uint32_t tstart = millis();

    while(_serial.available()) {
        if (_parser.parseByte(_serial.read()) == PingParser::NEW_MESSAGE) {
          debug("got response in %dms", millis() - tstart);
          handleMessage(&_parser.rxMsg);
          return &_parser.rxMsg;
        }
    }
    return nullptr;
  }
  
  size_t write(uint8_t* data, uint16_t length)
  {
    return _serial.write(data, length);
  }
  
  bool initialize()
  {
    return true;
  }
  
  PingMessage* request(enum Ping1DNamespace::msg_ping1D_id id)
  {
    ping_msg_ping1D_empty msg;
    msg.set_id(id);
    msg.updateChecksum();
    write(msg.msgData, msg.msgDataLength());
    return waitReply(id);
  }
  
  PingMessage* waitReply(enum Ping1DNamespace::msg_ping1D_id id, uint16_t timeout_ms=400)
  {
      uint32_t tstart = millis();
      while (millis() < tstart + timeout_ms) {
        PingMessage* pmsg = read();
        if (pmsg->message_id() == Ping1DNamespace::Nack) {
          ping_msg_ping1D_nack nack(*pmsg);
          debug("got NACK (%d) %s", nack.id_to_nack(), nack.err_msg());
          if (nack.id_to_nack() == id) {
            return nullptr;
          }
        }
        if (pmsg->message_id() == id) {
          debug("got response in %dms", millis() - tstart);
          return pmsg;
        }
      }
      debug("timed out waiting for id %d", id);
      return nullptr;
  }
  
  void handleMessage(PingMessage* pmsg)
  {
    switch (pmsg->message_id()) {
      case Ping1DNamespace::Nack:

      break;
      default:
      break;
    }
  }



  
  
private:
  SoftwareSerial& _serial;
  PingParser _parser;
};

