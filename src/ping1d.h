#pragma once

#include "Stream.h"
#include "pingmessage_all.h"

class Ping1D
{
public:
  Ping1D(Stream& ser, uint32_t baudrate) : _stream ( ser )
  {
  }
  
  PingMessage* read()
  {
    while(_stream.available()) {
        if (_parser.parseByte(_stream.read()) == PingParser::NEW_MESSAGE) {
          handleMessage(&_parser.rxMsg);
          return &_parser.rxMsg;
        }
    }
    return nullptr;
  }
  
  size_t write(uint8_t* data, uint16_t length)
  {
    return _stream.write(data, length);
  }
  
  bool initialize()
  {
    return true;
  }
  
  PingMessage* waitReply(enum Ping1DNamespace::msg_ping1D_id id, uint16_t timeout_ms=400)
  {
      uint32_t tstart = millis();
      while (millis() < tstart + timeout_ms) {
        PingMessage* pmsg = read();
        if (!pmsg) {
          return nullptr;
        }
        if (pmsg->message_id() == Ping1DNamespace::Voltage_5) {
                  ping_msg_ping1D_voltage_5 m(*pmsg);
                  debug("got voltage %d:", m.mvolts());
        
        }
        if (pmsg->message_id() == Ping1DNamespace::Nack) {
          ping_msg_ping1D_nack nack(*pmsg);
          debug("got NACK (%d) %s", nack.nacked_id(), nack.nack_msg());
          if (nack.nacked_id() == id) {
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
      case Ping1DNamespace::Voltage_5:
      {
          ping_msg_ping1D_voltage_5 m(*pmsg);
          debug("got voltage %d:", m.mvolts());
          break;
      }
      default:
      break;
    }
  }

  // ex ping_msg_ping1D_voltage_5 msg(*pd.request(Ping1DNamespace::Voltage_5));
  PingMessage* request(enum Ping1DNamespace::msg_ping1D_id id)
  {
    ping_msg_ping1D_empty msg;
    msg.set_id(id);
    msg.updateChecksum();
    write(msg.msgData, msg.msgDataLength());
    PingMessage* pmsg = waitReply(id);

    if (pmsg->message_id() == Ping1DNamespace::Distance_simple) {
      ping_msg_ping1D_distance_simple m(*pmsg);
      debug("got distance %d\tconfidence:", m.distance(), m.confidence());
    }
    
    if (pmsg->message_id() == Ping1DNamespace::Voltage_5) {
      ping_msg_ping1D_voltage_5 m(*pmsg);
      debug("got voltage %d:", m.mvolts());
    }
    
    return pmsg;
  }
  
  // ex auto msg = pd.request<ping_msg_ping1D_voltage_5>();
  template <typename T>
  T* request() {
    T resp; // todo there should be some other (static) way to get the message id?
    static ping_msg_ping1D_empty req;
    req.set_id(resp.message_id());
    req.updateChecksum();
    write(req.msgData, req.msgDataLength());
    return (T*)waitReply(resp.message_id());
  }
  
private:
  Stream& _stream;
  PingParser _parser;
};

