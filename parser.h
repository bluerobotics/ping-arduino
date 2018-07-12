#pragma once


// This class digests data and notifies owner when something interesting happens
class Parser
{
public:
    Parser();
    ~Parser();

    uint32_t parsed; // number of messages/packets successfully parsed
    uint32_t errors; // number of parse errors

    static const uint16_t bufSize = 256;
    uint8_t rxBuf[bufSize]; // input buffer
    uint8_t rxHead;
    
    virtual void parseBuffer(uint8_t* buf, uint16_t len) { } // asynchronous use, Child classes should signal when something happens ie. 'emit newMessage(Message m)'
    virtual uint8_t parseByte(uint8_t byte) { return 0; } // synchronous use, Child should return flags indicating incremental parse result/status
};

inline Parser::Parser() : rxHead(rxBuf)
{
}

inline Parser::~Parser()
{
}


