#include "Ping.h"

Ping::Ping(Stream *_stream) {
	stream = _stream;
	c = 1400;
}

void Ping::init() {
	//Set quiet mode
	// stream->write("@Q1\n");
	//Enter request mode
	this->sendConfig(1,0)

	//Wait to be sure that quiet mode is enabled
	//Not sure if necessary
	delay(100);
}

// I/O


void Ping::request(){
	this->sendRequest(0x01)
}

void Ping::read(){
	while (stream->available() >= 8){
		test_2 = stream->read();

		if ((test_1 == validation_1) && (test_2 == validation_2))
		{
			//Set up input buffer
			byte input_buffer[12] = {};
			input_buffer[0] = 66;
			input_buffer[1] = 82;

			//Read in header information
			for (int i = 2; i < 8; i++) {
				input_buffer[i] = byte(stream->read());
			}

			//TODO Determine message ID

			//TODO Determine message body size
			uint16_t messageBodySize = 0;

			//TODO Read in message body
			for (int i = 0; i < messageBodySize; i++){
				input_buffer[i] = byte(stream->read());
			}

			//Read in checksum
			for (int i = 0; i < 2; i++){
				input_buffer[i] = byte(stream->read());
			}

			// //Start counting at 2 to account for the start bytes
			// for (int i = 2; i < 10; i++){
			// 	input_buffer[i] = byte(stream->read());
			// }

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

void sendCommand(uint8_t commandID, String messageBody){
	//TODO implement

	//Construct header
	//Construct command body
	//Determine checksum

	//Assemble

	//Send

	//Wait for ACK / NACK
}


//Accessor Methods
/////////////////

void Ping::update() {
	//Request a new reading
	request();
	read();
}

uint32_t Ping::getDistance(){
	return (uint32_t)(new_sonar_report.smoothed_distance_mm);
}

uint8_t Ping::getConfidence(){
	return (uint8_t)(new_sonar_report.smoothed_distance_confidence_percent);
}

//Control Methods
/////////////////

void sendConfig(uint8_t rate, uint16_t cWater){
	//TODO implement
}

void sendRequest(uint16_t messageID){
	//TODO implement
	sendMessage(0xC1, messageString);
}

void sendRange(uint8_t auto, uint16_t start_mm, uint16_t range_mm){
	//TODO implement
	if (auto == 0){
		//Set Auto mode
	}
	else {
		//Set Manual Mode
		//Set distance range
	}
}

//Internal
/////////////////

bool validateChecksum(){
	//TODO replace the sonar report with the one that we're currently reading in. This won't work.
	uint32_t messageSize = sizeof(this->sonar_report_distance);
	uint32_t checksum = 0;

	for (int i = 0; i < messageSize; i++) {
		checksum += this->sonar_report_distance[i];
	}
	checksum = checksum % (2^16);
	return checksum
}
