#pragma once

#include "pingmessage.h"

class PingParser
{
public:
    PingParser() : rxMsg(bufSize), rxBuf(rxMsg.msgData) {}
    ~PingParser() {};

    uint16_t parsed = 0; // number of messages/packets successfully parsed
    uint16_t errors = 0; // number of parse errors
    
    PingMessage rxMsg;

    static const uint16_t bufSize = 256;
    uint8_t* rxBuf; // buffer for parsing. Use the internal buffer of rxMsg to save on mallocs (it needs only one!)
    uint8_t rxHead = 0; // index of first empty data
    
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

    uint8_t parseByte(uint8_t bite)
    {
        static volatile uint16_t payload_length = 0;
        static volatile uint8_t state = WAIT_START;

        switch(state) {
        case WAIT_START:
            rxHead = 0;
            if (bite == 'B') {
                rxBuf[rxHead++] = bite;
                state++;
            }
            break;
        case WAIT_HEADER:
            if (bite == 'R') {
                rxBuf[rxHead++] = bite;
                state++;
            } else {
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
            if(payload_length < bufSize - 8 - 2) {
              state++;
            } else {
              state = WAIT_START;
            }
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
            if(payload_length) {
                payload_length--;
            }

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

