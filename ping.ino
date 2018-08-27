#include "SoftwareSerial.h"
static char _debug_buffer[200];

SoftwareSerial pingSerial = SoftwareSerial(9, 10);
HardwareSerial& debugSerial = Serial;

#define debug(fmt, args ...)  do {sprintf(_debug_buffer, "[%s:%d]: " fmt "\n\r", __FUNCTION__, __LINE__, ## args); debugSerial.print(_debug_buffer);} while(0)

#include <pingmessage_all.h>
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
  debugSerial.println("sup");
}

bool waitResponse(uint16_t timeout_ms = 350)
{
  uint32_t tstart = millis();
  while (millis() < tstart + timeout_ms) {

      while(pingSerial.available()) { 

        if (p.parseByte(pingSerial.read()) == PingParser::NEW_MESSAGE) {
          debug("got response in %dms", millis() - tstart);
          return true;
        }
      }
  }
        debugSerial.println("timeout");
  //p.reset();
  return false;
}

void loop() {
  auto msg = pd.request<ping_msg_ping1D_voltage_5>();
  debug("> Pcb voltage: %d", msg->mvolts());

  while(1) if (pd.request(Ping1DNamespace::Voltage_5)) toggleLed();

  while (1); // stop
  
  static uint8_t counter = 0;
  static Ping1DNamespace::msg_ping1D_id requestIds[] = {
    Ping1DNamespace::Profile,
    Ping1DNamespace::Voltage_5,
    Ping1DNamespace::Processor_temperature,
    Ping1DNamespace::Fw_version,
  };
  static int requestIdsSize = sizeof(requestIds)/sizeof(requestIds[0]);
  counter++;
  counter = counter%requestIdsSize;

  ping_msg_ping1D_empty m;
  m.set_id(requestIds[counter]);
  m.updateChecksum();
  pingSerial.write(m.msgData, m.msgDataLength());
  
    if(waitResponse()) {
      switch(p.rxMsg.message_id()) {

        case Ping1DNamespace::Voltage_5: {
          ping_msg_ping1D_voltage_5 m(p.rxMsg);
          debug("> Pcb voltage: %d",m.mvolts());
          break;
        }

        case Ping1DNamespace::Nack: {
          ping_msg_ping1D_nack m(p.rxMsg);
          debug("> Nack text: %s", m.err_msg());
          break;
        }

        case Ping1DNamespace::Ascii_text: {
          ping_msg_ping1D_ascii_text m(p.rxMsg);
          debug("> Ascii text: %s", m.msg());
          break;
        }

        case Ping1DNamespace::Processor_temperature: {
          ping_msg_ping1D_processor_temperature m(p.rxMsg);
          debug("> Pcb temp: %d", m.temp());
          break;
        }

        case Ping1DNamespace::Fw_version: {
          ping_msg_ping1D_fw_version m(p.rxMsg);
          debug("> type: %d", m.device_type());
          debug("> model: %d", m.device_model());
          debug("> fw version: %d.%d", m.fw_version_major(), m.fw_version_minor());
          break;
        }

        case Ping1DNamespace::Profile: {
          ping_msg_ping1D_profile m(p.rxMsg);
          debug("> distance: %d", m.distance());
          debug("> confidence: %d", m.confidence());
          debug("> pulse_usec: %d", m.pulse_usec());
          debug("> ping_number: %d", m.ping_number());
          debug("> start_mm: %d", m.start_mm());
          debug("> length_mm: %d", m.length_mm());
          debug("> gain_index: %d", m.gain_index());
          debug("> num_points: %d", m.num_points());
          debug("> gain_index: %d", m.gain_index());
          for(int i =0; i < m.num_points(); i++) {
            debug("> data: %d", m.data()[i]);
          }
          break;
        }

        default:

          break;
      }

          toggleLed();
          debug("> id: %d\t Length: %d\t parsed: %d\t errors: %d", p.rxMsg.message_id(), p.rxMsg.payload_length(), p.parsed, p.errors);
    }
}
