#pragma once

#include "parser.h"
#include "pingmessage.h"

class PingParser : public Parser
{
public:
    PingParser()   : rxMsg(256), rxBuf(rxMsg.msgData)   {}

    PingMessage rxMsg;

    static const uint16_t bufSize = 256;
//    uint8_t rxBuf[256];
    uint8_t* rxBuf;
    uint8_t rxHead = 0;
    
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

    uint8_t parseByte(uint8_t ibyte) override
    {
        static volatile uint16_t payload_length = 0;
        static volatile uint8_t state = WAIT_START;
        static volatile uint16_t msgId = 0;
        debug("msgId: %d\t rxHead: %d\t byte: %d \t state: %d", msgId, rxHead, ibyte, state);
        switch(state) {
        case WAIT_START:
            rxHead = 0;
            msgId = 0;
            if (ibyte == 'B') {
                rxBuf[rxHead++] = ibyte;
                state++;
            } else {
                errors++;
            }
            break;
        case WAIT_HEADER:
            if (ibyte == 'R') {
                rxBuf[rxHead++] = ibyte;
                state++;
            } else {
                errors++;
                state = WAIT_START;
            }
            break;
        case WAIT_LENGTH_L:
            rxBuf[rxHead++] = ibyte;
            payload_length = (uint8_t)ibyte;
            state++;
            break;
        case WAIT_LENGTH_H:
            rxBuf[rxHead++] = ibyte;
            payload_length = (ibyte << 8) | payload_length;
            state++;
            break;
        case WAIT_MSG_ID_L:
            rxBuf[rxHead++] = ibyte;
            msgId = (uint8_t)ibyte;
            state++;
            break;
        case WAIT_MSG_ID_H:
            rxBuf[rxHead++] = ibyte;
             msgId = (ibyte << 8) | msgId;
            state++;
            break;
        case WAIT_SRC_ID:
        case WAIT_DST_ID:
            rxBuf[rxHead++] = ibyte;
            state++;
            break;
        case WAIT_PAYLOAD:
            rxBuf[rxHead++] = ibyte;
            payload_length--;

            if(payload_length == 0) {
                state++;
            }
            break;
        case WAIT_CHECKSUM_L:
            rxBuf[rxHead++] = ibyte;
            state++;
            break;
        case WAIT_CHECKSUM_H:
            rxBuf[rxHead++] = ibyte;
            //rxMsg = PingMessage(rxBuf);

            payload_length = 0;
            state = WAIT_START;

            //debug("check: %d\t checksum: %u\t cs: %u \t rxMsg.verifyChecksum(): %d", rxMsg.msgDataLength(), rxMsg.checksum(), rxMsg.calculateChecksum(), rxMsg.verifyChecksum());

            if (!rxMsg.verifyChecksum()) {
                errors++;
                debug("\nfail %d", errors);

            } else {
                debug("verified");
                parsed++;
                return NEW_MESSAGE;
            }
        }
        return state;
    }
};

