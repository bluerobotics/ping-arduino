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
	uint16_t read_attempt_count = 0;
	while ((Serial1.available() >= MIN_PACKET_LENGTH) && (read_attempt_count <= SERIAL_READ_TIMEOUT)){
		test_2 = Serial1.read();

		if ((test_1 == validation_1) && (test_2 == validation_2))
		{
			//Set up input buffer
			header_buffer[0] = 66;
			header_buffer[1] = 82;

			//Read header information
			for (int i = 2; i < 8; i++) {
				header_buffer[i] = byte(Serial1.read());
			}
			memcpy(&message_header, &header_buffer, sizeof(message_header));
			//Determine message ID
			uint16_t messageID = message_header.messageID;
			//Determine message body size
			payload_size = message_header.length;

			//Now that we know the message size, wait for the entire message to hit the buffer
			uint16_t rx_count = 0;
			while ((Serial1.available() <= (payload_size + sizeof(checksum_buffer))) && (rx_count <= WAIT_FOR_RX_TIMEOUT)) {
				rx_count++;
				delay(1);
			}

			//Read Payload
			for (int i = 0; i < payload_size; i++){
				payload_buffer[i] = byte(Serial1.read());
			}

			//Read checksum
			for (int i = 0; i < 2; i++){
				checksum_buffer[i] = byte(Serial1.read());
			}
			memcpy(&message_checksum, &checksum_buffer, sizeof(message_checksum));

			bool checksum_match = validateChecksum();
			if (!checksum_match) {
				//Write Header
				for (int i = 0; i < sizeof(header_buffer); i++) {
					Serial.print(header_buffer[i]);
					Serial.print("|");
				}
				//Write Payload [keep in mind, it is variable in length]
				for (int i = 0; i < payload_size; i++) {
					Serial.print(payload_buffer[i]);
					Serial.print("|");
				}
				//Write Checksum
				for (int i = 0; i < sizeof(checksum_buffer); i++) {
					Serial.print(checksum_buffer[i]);
					Serial.print("|");
				}
				Serial.print("\n\n");

				cleanup();
				return;
			}



			cleanup();
		}
		else
		{
			//Invalid start sequence
			//Move byte to the first index
			read_attempt_count++;
			test_1 = test_2;
		}
	}
}

void Ping::sendMessage(uint16_t m_id){
	//Construct header
	buildHeader(payload_size, m_id);
	memcpy(&header_buffer, &message_header, sizeof(message_header));

	//Determine checksum
	buildChecksum();

	//Send
	//Write Header
	for (int i = 0; i < sizeof(header_buffer); i++)
		Serial1.write(header_buffer[i]);

	//Write Payload [keep in mind, it is variable in length]
	for (int i = 0; i < payload_size; i++)
		Serial1.write(payload_buffer[i]);

	//Write Checksum
	for (int i = 0; i < sizeof(checksum_buffer); i++)
		Serial1.write(checksum_buffer[i]);

	//Write Header for viewing
	// for (int i = 0; i < sizeof(header_buffer); i++) {
	// 	Serial.print(header_buffer[i]);
	// 	Serial.print("|");
	// }
	// //Write Payload [keep in mind, it is variable in length]
	// for (int i = 0; i < payload_size; i++) {
	// 	Serial.print(payload_buffer[i]);
	// 	Serial.print("|");
	// }
	// //Write Checksum
	// for (int i = 0; i < sizeof(checksum_buffer); i++) {
	// 	Serial.print(checksum_buffer[i]);
	// 	Serial.print("|");
	// }
	// Serial.print("\n");

}

//Accessor Methods
/////////////////

void Ping::update() {
	//Request a new reading
	//TODO this is hard coded for now

	sendRequest(0x3, 1);
	read();

	//TODO If something was read, update local vars
}

uint32_t Ping::getDistance(){
	//TODO re implement
	return 0;
}

uint8_t Ping::getConfidence(){
	//TODO re implement
	return 0;
}

//Control Methods
/////////////////

void Ping::sendConfig(uint8_t rate, uint16_t cWater){
	//TODO implement
}

void Ping::sendRequest(uint16_t m_id, uint16_t m_rate){
	message_request.requestID = m_id;
	message_request.rate = m_rate;

	//Copy the message into the payload buffer
	memcpy(&payload_buffer, &message_request, sizeof(message_request));
	payload_size = sizeof(message_request);

	sendMessage(0x101);

	cleanup();
}

void Ping::sendRange(uint8_t auto, uint16_t start_mm, uint16_t range_mm){
	//TODO re implement
}

//Internal
/////////////////

bool Ping::validateChecksum(){
	uint16_t calculated_checksum = 0;
	uint16_t message_checksum = 0;
	memcpy(&message_checksum, &checksum_buffer, sizeof(checksum_buffer));

	//Header
	for (int i = 0; i < 8; i++)
		calculated_checksum += header_buffer[i];

	//Payload
	for (int i = 0; i < payload_size; i++)
		calculated_checksum += payload_buffer[i];

	//Calculate
 	calculated_checksum = calculated_checksum & 0xffff;

	bool match = (calculated_checksum == message_checksum);
	if (!match) {
		Serial.print("Checksum Mismatch! Actual: ");
		Serial.print(calculated_checksum);
		Serial.print(" Claimed: ");
		Serial.print(message_checksum);
		Serial.println("");
	}
	return match;
}

void Ping::buildHeader(uint16_t payloadLength, uint16_t messageID) {
	message_header.start_byte1 = 'B';
	message_header.start_byte2 = 'R';
	message_header.length = payloadLength;
	message_header.messageID = messageID;
}

void Ping::buildChecksum (){
	uint16_t m_checksum = 0;

	//Header
	for (int i = 0; i < 8; i++)
		m_checksum += header_buffer[i];

	//Payload
	for (int i = 0; i < payload_size; i++)
		m_checksum += payload_buffer[i];

	//Calculate
 	m_checksum = m_checksum & 0xffff;
	memcpy(&checksum_buffer, &m_checksum, sizeof(m_checksum));
}

void Ping::cleanup(){
	test_1 = 0;
	test_2 = 0;
	for (int i = 0; i < sizeof(header_buffer); i++) {
		header_buffer[i] = 0;
	}
	for (int i = 0; i < sizeof(payload_buffer); i++) {
		payload_buffer[i] = 0;
	}
	payload_size = 0;
	for (int i = 0; i < sizeof(checksum_buffer); i++) {
		checksum_buffer[i] = 0;
	}
}

// //Write Header
// for (int i = 0; i < sizeof(header_buffer); i++) {
// 	Serial.print(header_buffer[i]);
// 	Serial.print("|");
// }
// //Write Payload [keep in mind, it is variable in length]
// for (int i = 0; i < payload_size; i++) {
// 	Serial.print(payload_buffer[i]);
// 	Serial.print("|");
// }
// //Write Checksum
// for (int i = 0; i < sizeof(checksum_buffer); i++) {
// 	Serial.print(checksum_buffer[i]);
// 	Serial.print("|");
// }
// Serial.print("\n");
