#include "Ping.h"

Ping::Ping(Stream *_stream) {
	stream = _stream;
	c = 1400;
}

void Ping::init() {
	//Set quiet mode
	// stream->write("@Q1\n");

	//TODO Update this to match new protocol
	//Enter request mode
	//this->sendConfig(1,0);

	//TODO update status and stuff on initialization

	//Wait to be sure that quiet mode is enabled
	//Not sure if necessary
	delay(100);
}

// I/O
//////

void Ping::read(){
	//Smallest possible message is 10 bytes
	while (stream->available() >= 10){
		test_2 = stream->read();

		if ((test_1 == validation_1) && (test_2 == validation_2))
		{
			//Set up input buffer
			byte header_buffer[8] = {};
			header_buffer[0] = 66;
			header_buffer[1] = 82;

			//Read in header information
			for (int i = 2; i < 8; i++) {
				header_buffer[i] = byte(stream->read());
			}
			//Store header information
			memcpy(&message_header, &header_buffer, sizeof(message_header));

			//Determine message ID
			uint16_t messageID = message_header.messageID;
			//Determine message body size
			uint16_t messageBodySize =  message_header.length;

			//Store message body
			//TODO this can't be variable length. Find another data structure
			// byte message_buffer[messageBodySize] = {};
			//
			// // Read in message body
			// for (int i = 0; i < messageBodySize; i++){
			// 	message_buffer[i] = byte(stream->read());
			// }

			byte checksum_buffer[2] = {};

			//Read in checksum
			for (int i = 0; i < 2; i++){
				checksum_buffer[i] = byte(stream->read());
			}
			memcpy(&message_checksum, &checksum_buffer, sizeof(message_checksum));


			//Take recorded data and save it
			//memcpy(&new_sonar_report, &input_buffer, sizeof(new_sonar_report));
		}
		else
		{
			//Invalid start sequence
			//Move byte to the first index
			test_1 = test_2;
		}
	}
}

void Ping::sendMessage(){
	//TODO implement

	//Determine length of payload
	uint16_t payloadLength = 0;
	uint16_t messageID = 0;

	//Construct header
	template_message_header* new_message_header;
	buildHeader(new_message_header, payloadLength, messageID);

	printHeader(new_message_header);

	Serial.print("\n");
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
	//request(0x3, 1);
	//read();

	sendMessage();

	//TODO If something was read, update local vars
}

uint32_t Ping::getDistance(){
	//TODO re implement
	//return (uint32_t)(new_sonar_report.smoothed_distance_mm);
	return 0;
}

uint8_t Ping::getConfidence(){
	//TODO re implement
	//return (uint8_t)(new_sonar_report.smoothed_distance_confidence_percent);
	return 0;
}

//Control Methods
/////////////////

void sendConfig(uint8_t rate, uint16_t cWater){
	//TODO implement
}

void sendRequest(uint16_t messageID){
	//TODO re implement
	//sendMessage(0xC1, messageString);
}

void sendRange(uint8_t auto, uint16_t start_mm, uint16_t range_mm){
	//TODO re implement
	// if (auto == 0){
	// 	//Set Auto mode
	// }
	// else {
	// 	//Set Manual Mode
	// 	//Set distance range
	// }
}

//Internal
/////////////////

bool validateChecksum(){
	//TODO replace the sonar report with the one that we're currently reading in. This won't work.
	// uint32_t messageSize = sizeof(this->sonar_report_distance);
	// uint32_t checksum = 0;
	//
	// for (int i = 0; i < messageSize; i++) {
	// 	checksum += this->sonar_report_distance[i];
	// }
	//checksum = checksum % (2^16);
	return false;
}

void Ping::buildHeader(template_message_header* message_header, uint16_t payloadLength, uint16_t messageID) {
	message_header->start_byte1 = 'B';
	message_header->start_byte2 = 'R';
	message_header->length = payloadLength;
	message_header->messageID = messageID;
}

void Ping::printHeader(template_message_header* message_header){
	Serial.print(message_header->start_byte1);
}

bool Ping::buildChecksum (){
	//TODO implement as a mirror of the python library
	return false;
}
