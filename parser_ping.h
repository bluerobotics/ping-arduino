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

    // byte is reserved word in arduino :(
    uint8_t parseByte(uint8_t bite) override
    {
        static volatile uint16_t payload_length = 0;
        static volatile uint8_t state = WAIT_START;
        //debug("rxHead: %d\t byte: %d \t state: %d", rxHead, bite, state);
        switch(state) {
        case WAIT_START:
            rxHead = 0;
            if (bite == 'B') {
                rxBuf[rxHead++] = bite;
                state++;
            } else {
                errors++;
            }
            break;
        case WAIT_HEADER:
            if (bite == 'R') {
                rxBuf[rxHead++] = bite;
                state++;
            } else {
                errors++;
                state = WAIT_START;
            }
            break;
        case WAIT_LENGTH_L:
            rxBuf[rxHead++] = bite;
            payload_length = (uint8_t)bite;
            state++;
            break;
        case WAIT_LENGTH_H:
            rxBuf[rxHead++] = bite;
            payload_length = (bite << 8) | payload_length;
            state++;
            break;
        case WAIT_MSG_ID_L:
        case WAIT_MSG_ID_H:
        case WAIT_SRC_ID:
        case WAIT_DST_ID:
            rxBuf[rxHead++] = bite;
            state++;
            break;
        case WAIT_PAYLOAD:
            rxBuf[rxHead++] = bite;
            payload_length--;

            if(payload_length == 0) {
                state++;
            }
            break;
        case WAIT_CHECKSUM_L:
            rxBuf[rxHead++] = bite;
            state++;
            break;
        case WAIT_CHECKSUM_H:
            rxBuf[rxHead++] = bite;
            rxMsg = PingMessage(rxBuf, rxHead);
            for(uint8_t i = 0; i < rxMsg.msgDataLength(); i++) {
              Serial3.print("MsgNow1: "); Serial3.print(i); Serial3.print('\t'); Serial3.print(rxMsg.msgData[i]); Serial3.print('\t'); Serial3.println((uint16_t)&rxMsg.msgData[i]);
            }
            payload_length = 0;
            state = WAIT_START;

            if (!rxMsg.verifyChecksum()) {
                errors++;
            } else {
                parsed++;
                return NEW_MESSAGE;
            }
        }
        return state;
    }

};

