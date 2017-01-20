
/* Blue Robotics Ping Sonar Library
------------------------------------------------------------

Title: Blue Robotics Ping Sonar Library
Description: This library provides utilities to communicate with and to
read data Blue Robotics Ping1D sonar sensor.
Authors: 	Nick Nothom, Digi Labs LLC
					Rustom Jehangir, Blue Robotics Inc.
-------------------------------
The MIT License (MIT)
Copyright (c) 2016 Blue Robotics Inc.
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-------------------------------*/


#ifndef PING_H_BLUEROBOTICS
#define PING_H_BLUEROBOTICS

#include "Arduino.h"
#include <Stream.h>

//TODO update this for new protocol
#define MIN_PACKET_LENGTH 16

//Messages
//////////
#define REQUEST_MINIMAL "@D\n"
#define AUTO_ENABLE
#define AUTO_DISABLE

class Ping {
public:
	static const float Pa = 100.0f;
	Ping(Stream *stream);
	void init();

	// I/O
	//////

	//Request a new reading from Ping
	void request();

	//Read in a new packet
	void read();

	void sendMessage(uint16_t m_id);

	//Accessor Methods
	//////////////////

	//Request + Read
	void update();

	//Distance, mm
	uint32_t getDistance();

	//Confidence in distance measurement, as a percentage
	uint8_t getConfidence();

	//Control Methods
	/////////////////

	//Set speed of sound based on fluid type
	void sendConfig(uint8_t rate, uint16_t cWater);

	//Set initial configuration options
	void sendRequest(uint16_t m_id, uint16_t m_rate);

	//Set the range that Ping will scan in
	void sendRange(uint8_t auto, uint16_t start_mm, uint16_t range_mm);

private:
	float c;

	//Characters pulled from serial buffer to check for start sequence
	char test_1 = 0;
	char test_2 = 0;

	//Characters to validate start sequences
	char validation_1 = 66;
	char validation_2 = 82;

	//Metadata Structures
	/////////////////////

	struct template_header {
		uint8_t  start_byte1; //B
		uint8_t  start_byte2; //R
		uint16_t length;      //Payload length
		uint16_t messageID;   //ID of message being sent
		uint16_t reserved;    //Will possibly be used in the future
	} message_header;

	uint16_t message_checksum;

	//Message Structures
	/////////////////////

	struct template_nack {
		uint16_t id;                      //ID of message being nacked
		char error[256];                  //Error Message
	} message_nack;

	struct template_distance {
		uint32_t raw_distance;             //Last ping range mm
		uint32_t smoothed_distance;        //Distance to item, mm
		uint8_t  confidence;               //Confidence percentage
	} message_distance ;

	struct template_general_info {
		uint16_t fw_version_major;
		uint16_t fw_version_minor;
		uint16_t voltage;
		uint16_t  error;
	} message_general_info ;

	struct template_ascii_text {
		char ascii_string[256];             //Null Terminated
	} message_ascii_text ;

	struct template_request {
		uint16_t requestID;
		uint16_t rate;
	} message_request ;

	byte payload_buffer[32];
	uint16_t payload_size = 0;

	byte header_buffer[8];

	byte checksum_buffer[2];

	Stream *stream;

	bool validateChecksum();
	void buildChecksum();

	void buildHeader(uint16_t payloadLength, uint16_t messageID);
	//void buildHeader(template_header* headerPtr, uint16_t payloadLength, uint16_t messageID);
	void printHeader();
};

#endif
