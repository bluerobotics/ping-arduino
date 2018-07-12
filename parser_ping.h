#pragma once

#include "parser.h"
#include "pingmessage.h"

class PingParser : public Parser
{
public:
    PingParser() {}

    PingMessage rxMsg;

    enum {
        NEW_MESSAGE,   // Just got a complete checksum-verified message
        WAIT_START,    // Waiting for the first character of a message 'B'
        WAIT_HEADER,   // Waiting for the second character in the two-character sequence 'BR'
        WAIT_LENGTH_L, // Waiting for the low byte of the payload length field
        WAIT_LENGTH_H, // Waiting for the high byte of the payload length field
        WAIT_MSG_ID_L, // Waiting for the low byte of the payload id field
        WAIT_MSG_ID_H, // Waiting for the high byte of the payload id field
        WAIT_SRC_ID,   // Waiting for the source device id
        WAIT_DST_ID,   // Waiting for the destination device id
        WAIT_PAYLOAD,  // Waiting for the last byte of the payload to come in
        WAIT_CHECKSUM_L, // Waiting for the checksum low byte
        WAIT_CHECKSUM_H, // Waiting for the checksum high byte
    };

    void parseBuffer(uint8_t* buf, uint16_t len) override
    {
        for(uint16_t i = 0; i < len; i++) {
            parseByte(buf[i]);
        }
    }

    uint8_t parseByte(uint8_t byte) override
    {
        static volatile uint16_t payload_length = 0;
        static volatile uint8_t state = WAIT_START;
        switch(state) {
        case WAIT_START:
            rxHead = 0;
            if (byte == 'B') {
                rxBuf[rxHead++] = byte;
                state++;
            } else {
                errors++;
            }
            break;
        case WAIT_HEADER:
            if (byte == 'R') {
                rxBuf[rxHead++] = byte;
                state++;
            } else {
                errors++;
                state = WAIT_START;
            }
            break;
        case WAIT_LENGTH_L:
            rxBuf[rxHead++] = byte;
            payload_length = (uint8_t)byte;
            state++;
            break;
        case WAIT_LENGTH_H:
            rxBuf[rxHead++] = byte;
            payload_length = (byte << 8) | payload_length;
            state++;
            break;
        case WAIT_MSG_ID_L:
        case WAIT_MSG_ID_H:
        case WAIT_SRC_ID:
        case WAIT_DST_ID:
            rxBuf[rxHead++] = byte;
            state++;
            break;
        case WAIT_PAYLOAD:
            rxBuf[rxHead++] = byte;
            if (--payload_length == 0) {
                state++;
            }
            break;
        case WAIT_CHECKSUM_L:
            rxBuf[rxHead++] = byte;
            state++;
            break;
        case WAIT_CHECKSUM_H:
            rxBuf[rxHead++] = byte;
        	  rxMsg = PingMessage(rxBuf, rxHead);
            for(uint8_t i = 0; i < rxMsg.msgDataLength(); i++) {
              Serial3.print("MsgNow1: "); Serial3.print(i); Serial3.print('\t'); Serial3.print(rxMsg.msgData[i]); Serial3.print('\t'); Serial3.println((uint16_t)&rxMsg.msgData[i]);
            }
            payload_length = 0;
            state = WAIT_START;
            Serial3.println("msgrx");
            for(uint8_t i = 0; i < rxHead; i++) {
              Serial3.print("RxBuf: "); Serial3.print(i); Serial3.print('\t'); Serial3.print(rxBuf[i]); Serial3.println('\t');
            }
            uint16_t cs = rxMsg.calculateChecksum();
            Serial3.print("check: "); Serial3.print(rxMsg.msgDataLength()); Serial3.print(' '); Serial3.print(rxMsg.checksum()); Serial3.print(' '); Serial3.println(cs);
            for(uint8_t i = 0; i < rxMsg.msgDataLength(); i++) {
              Serial3.print("MsgNow2: "); Serial3.print(i); Serial3.print('\t'); Serial3.print(rxMsg.msgData[i]); Serial3.println('\t');
            }
//            if (!rxMsg.verifyChecksum()) {
//                errors++;
//            } else {
//                parsed++;
//                return NEW_MESSAGE;
//            }
        }

        return state;
    }

};

