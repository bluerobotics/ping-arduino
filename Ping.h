
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

#define MIN_PACKET_LENGTH 16

class Ping {
public:
	static const float Pa = 100.0f;

	Ping(Stream *stream);

	void init();

	/** Set speed of sound based on fluid type.
	 */
	void setSpeedOfSound(float speed);

	/** The read from the serial buffer. Reads all messages until there
	 *  are not enough bytes available to read another.
	 */
	void read();

	void request();

	/** Altitude above bottom, meters
	 */
	float altitude();

	/** Confidence of the altitude measurement, 0-100
	 */
	uint8_t confidence();

private:
	float c;

	char test_1 = 0;
	char test_2 = 0;
	char validation_1 = 68;
	char validation_2 = 67;

	//Char array for incoming bytes
	byte input[1024];
	//Points to where data is being placed
	int p = 0;

	struct sonar_report {
		char	s1;												// 's'
		char	s2;												// 's'
		uint16_t	fw_version_major;
		uint16_t	fw_version_minor;
		int16_t 	num_results;								// SAMPLES_TO_REPORT
		int32_t	supply_millivolts;
		int32_t	start_mm;
		int32_t	length_mm;
		int32_t	this_ping_depth_mm;
		int32_t	smoothed_depth_mm;
		int16_t	smoothed_depth_confidence_percent;	// 1..100
		int16_t	ping_duration_usec;						// pulse_usec;
		int16_t	goertzel_N;
		int16_t	goertzel_m;
		int32_t	analog_gain;
		uint32_t	ping_number;
		uint32_t	timestamp_msec;
		int16_t	index_of_bottom_result;
		uint16_t	results[200];			// actually num_results, but hard coded size for now
		char	e1;												// 'e'
		char	e2;												// 'e'
	};

	struct sonar_report_minimal {
		char    s1; // 'D'
		char    s2; // 'C'
		int16_t    smoothed_depth_confidence_percent;    // 1..100
		int32_t    smoothed_depth_mm;                        //
		char    e1; // 'e'
		char    e2; // 'e'
	};

	sonar_report_minimal new_sonar_report;

	Stream *stream;

	/** Performs calculations to adjust measurement based on speed of
	  * sound.
	  */
	void calculate();
};

#endif
