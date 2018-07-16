#pragma once


// This class digests data and notifies owner when something interesting happens
class Parser
{
public:
    Parser();
    ~Parser();

    uint32_t parsed = 0; // number of messages/packets successfully parsed
    uint32_t errors = 0; // number of parse errors

    virtual void parseBuffer(uint8_t* buf, uint16_t len) { }
    virtual uint8_t parseByte(uint8_t ibyte) { return 0; }
};

inline Parser::Parser()
{
}

inline Parser::~Parser()
{
}


