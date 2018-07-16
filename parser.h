#pragma once


// This class digests data and notifies owner when something interesting happens
class Parser
{
public:
    Parser();
    ~Parser();

    uint32_t parsed = 0; // number of messages/packets successfully parsed
    uint32_t errors = 0; // number of parse errors

    static const uint16_t bufSize = 256;
    uint8_t rxBuf[bufSize]; // input buffer
    uint8_t rxHead = 0;

    virtual void parseBuffer(uint8_t* buf, uint16_t len) { } // asynchronous use, Child classes should signal when something happens ie. 'emit newMessage(Message m)'
    virtual uint8_t parseByte(uint8_t bite) { return 0; } // synchronous use, Child should return flags indicating incremental parse result/status
};

inline Parser::Parser() : rxHead(0)
{
}

inline Parser::~Parser()
{
}


