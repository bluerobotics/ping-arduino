#include "Ping.h"

Ping::Ping(Stream *_stream)
{
	stream = _stream;
	c = 1400;
}

void Ping::init()
{
	//Wait for boot
	delay(100);

	updateInfo();
}

// I/O
//////

//Reads and returns the read message ID
uint16_t Ping::read()
{
	uint16_t read_attempt_count = 0;
	while ((Serial1.available() >= MIN_PACKET_LENGTH) && (read_attempt_count <= SERIAL_READ_TIMEOUT))
	{
		test_2 = Serial1.read();

		if ((test_1 == validation_1) && (test_2 == validation_2))
		{
			//Set up input buffer
			header_buffer[0] = 66;
			header_buffer[1] = 82;

			//Read header information
			for (int i = 2; i < 8; i++)
				header_buffer[i] = byte(Serial1.read());
			memcpy(&message_header, &header_buffer, sizeof(message_header));

			//Determine message ID
			uint16_t messageID = message_header.messageID;
			//Determine message body size
			payload_size = message_header.length;

			//Now that we know the message size, wait for the entire message to hit the buffer
			uint16_t rx_count = 0;
			while ((Serial1.available() <= (payload_size + sizeof(checksum_buffer))) && (rx_count <= WAIT_FOR_RX_TIMEOUT))
			{
				rx_count++;
				delay(1);
			}
			if (rx_count >= WAIT_FOR_RX_TIMEOUT)
			{
				if (DEBUG)
				{
					Serial.print("Serial Receive Timeout");
				}
				return 0;
			}
			//Read Payload
			for (int i = 0; i < payload_size; i++)
				payload_buffer[i] = byte(Serial1.read());

			//Read checksum
			for (int i = 0; i < 2; i++)
				checksum_buffer[i] = byte(Serial1.read());

			//Validate Checksum
			memcpy(&message_checksum, &checksum_buffer, sizeof(message_checksum));
			bool checksum_match = validateChecksum();

			if (!checksum_match)
			{
				cleanup();
				return 0;
			}

			return messageID;
		}
		else
		{
			//Invalid start sequence
			//Move byte to the first index
			read_attempt_count++;
			test_1 = test_2;
		}
	}
	if (read_attempt_count >= SERIAL_READ_TIMEOUT)
		return 0;
}

void Ping::sendMessage(uint16_t m_id)
{
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
}

//Accessor Methods
/////////////////

void Ping::update()
{
	//Request a new reading
	sendRequest(0x3, 1);

	//Read
	uint16_t receivedID = read();

	//Deal with it
	handleMessage(receivedID);
}

uint32_t Ping::getDistance()
{
	return ping_smoothed_distance;
}

uint8_t Ping::getConfidence()
{
	return ping_confidence;
}

void Ping::updateInfo()
{
	//Request a new reading
	sendRequest(0x6, 1);

	//Read
	uint16_t receivedID = read();

	//Deal with it
	handleMessage(receivedID);
}


//Control Methods
/////////////////

void Ping::sendConfig(uint8_t rate, uint16_t cWater)
{
	//TODO implement
}

void Ping::sendRequest(uint16_t m_id, uint16_t m_rate)
{
	template_request m_message;
	m_message.requestID = m_id;
	m_message.rate = m_rate;

	memcpy(&payload_buffer, &m_message, sizeof(m_message));
	payload_size = sizeof(m_message);

	sendMessage(0x101);
	cleanup();
}

void Ping::sendRange(uint16_t m_start_mm, uint16_t m_range_mm)
{
	template_range m_message;
	m_message.start_mm = m_start_mm;
	m_message.length_mm = m_range_mm;

	memcpy(&payload_buffer, &m_message, sizeof(m_message));
	payload_size = sizeof(m_message);

	sendMessage(0x102);
	cleanup();
}

//Internal
/////////////////

bool Ping::validateChecksum()
{
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
	if (DEBUG && !match)
	{
		Serial.print("Checksum Mismatch! Actual: ");
		Serial.print(calculated_checksum);
		Serial.print(" Claimed: ");
		Serial.print(message_checksum);
		Serial.println("");
	}
	return match;
}

void Ping::buildHeader(uint16_t payloadLength, uint16_t messageID)
{
	message_header.start_byte1 = 'B';
	message_header.start_byte2 = 'R';
	message_header.length = payloadLength;
	message_header.messageID = messageID;
}

void Ping::buildChecksum ()
{
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

void Ping::handleMessage(uint16_t m_id)
{
	switch (m_id)
	{
		case 0x1:
			//TODO Handle ACK
			break;
		case 0x2:
			//TODO Handle NACK
			break;

		case 0x3:
		{
			template_distance m_message;
			memcpy(&m_message, &payload_buffer, sizeof(m_message));
			ping_smoothed_distance = m_message.smoothed_distance;
			ping_raw_distance = m_message.raw_distance;
			ping_confidence = m_message.confidence;
			break;
		}

		case 0x6:
		{
			template_general_info m_message;
			memcpy(&m_message, &payload_buffer, sizeof(m_message));
			ping_fw_version_major = m_message.fw_version_major;
			ping_fw_version_minor = m_message.fw_version_minor;
			ping_voltage = m_message.voltage;
			ping_msec_per_ping = m_message.msec_per_ping;
			ping_is_auto = m_message.is_auto;
			break;
		}

		case 0x7:
		{
			template_ascii_text m_message;
			memcpy(&m_message, &payload_buffer, sizeof(payload_size));
			if(DEBUG)
			{
				Serial.print(m_message.ascii_string);
			}
			break;
		}

		default:
			break;
	}
	cleanup();
}

//Useful for debugging
void Ping::printMessage(){
	//Print Message
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
	Serial.println("");
}

void Ping::cleanup()
{
	test_1 = 0;
	test_2 = 0;
	for (int i = 0; i < sizeof(header_buffer); i++)
		header_buffer[i] = 0;

	for (int i = 0; i < sizeof(payload_buffer); i++)
		payload_buffer[i] = 0;

	payload_size = 0;
	for (int i = 0; i < sizeof(checksum_buffer); i++)
		checksum_buffer[i] = 0;
}
