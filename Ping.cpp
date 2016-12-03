#include "Ping.h"

Ping::Ping(Stream *_stream) {
	stream = _stream;
	c = 1400;
}

void Ping::init() {
	//Set quiet mode
	stream->write("@Q1\n");

	//Wait to be sure that quiet mode is enabled
	//Not sure if necessary
	delay(100);
}

void Ping::update() {
	//Request a new reading
	request();
	read();
}

void Ping::read(){
	while (stream->available() >= 8){
		test_2 = stream->read();

		if ((test_1 == validation_1) && (test_2 == validation_2))
		{
			//Set up input buffer
			byte input_buffer[12] = {};
			input_buffer[0] = 68;
			input_buffer[1] = 67;

			//Start counting at 2 to account for the start bytes
			for (int i = 2; i < 10; i++){
				input_buffer[i] = byte(stream->read());
			}

			//Take recorded data and save it
			memcpy(&new_sonar_report, &input_buffer, sizeof(new_sonar_report));
		}
		else
		{
			//Invalid start sequence
			//Move byte to the first index
			test_1 = test_2;
		}
	}
}

//Accessor Methods
/////////////////

float Ping::getDepth(){
	return (float)(new_sonar_report.smoothed_depth_mm);
}

float Ping::getConfidence(){
	return (float)(new_sonar_report.smoothed_depth_confidence_percent);
}

//Control Methods
/////////////////

void setSpeedOfSound(float speed){
		//TODO implement
		//stream->write();
}

void setRange(uint8_t auto, uint16_t start, uint16_t range){
	//TODO implement
	// if (auto == 0){
	// 	//Set Manual mode
	// 	//Set depth range
	// }
	// else {
	// 	//Set Auto mode
	// }
}

//Configuration
///////////////

void Ping::request(){
	stream->write("@D\n");
}

void Ping::setSpeedOfSound(float speed) {
	c = speed;
}

void Ping::calculate() {
	//TODO implement
}

bool validateCRC(){
	// uint16_t crc = 0xFFFF;
	//
  // for ( unsigned int i = 0 ; i < length ; i++ ) {
  //   msgBuffer[i] = Serial->read();
  //   crc = _crc_ccitt_update(crc,msgBuffer[i]);
  // }
  // crc = ~crc;
	//
  // Serial.print("CRC: ");Serial.println(crc);
}
