
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
#include <util/crc16.h>
#include <Stream.h>

//TODO update this for new protocol
#define MIN_PACKET_LENGTH 16


//Messages
//////////
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

	//Request + Read
	void update();


	//Accessor Methods
	//////////////////

	//Distance, mm
	float getDistance();

	//Confidence in distance measurement, as a percentage
	float getConfidence();


	//Control Methods
	/////////////////

	//Set speed of sound based on fluid type
	void setSpeedOfSound(float speed);

	//Set initial configuration options
	void setConfiguration(uint8_t rate, uint16_t c);

	//Set the range that Ping will scan in
	void setRange(uint8_t auto, uint16_t start_mm, uint16_t range_mm);

	//Special debug options for testing
	void setDebugOptions(uint8_t raw, uint8_t auto, uint16_t gain, uint16_t c);

private:
	float c;

	bool validateCRC();

	//Characters pulled from serial buffer to check for start sequence
	char test_1 = 0;
	char test_2 = 0;

	//Characters to validate start sequences
	char validation_1 = 68;
	char validation_2 = 67;

	//V1 Sonar Struct
	struct sonar_report_minimal {
		char    s1; // 'D'
		char    s2; // 'C'
		int16_t    smoothed_distance_confidence_percent;    // 1..100
		int32_t    smoothed_distance_mm;                        //
		char    e1; // 'e'
		char    e2; // 'e'
	} new_sonar_report ;

	Stream *stream;

	//Performs calculations to adjust measurement based on speed of
	void calculate();
};

#endif
