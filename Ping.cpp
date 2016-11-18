#include "Ping.h"

Ping::Ping(Stream *_stream) {
	stream = _stream;
	c = 1400;
}

void Ping::init() {
	//Set quiet mode
	stream->write("@Q1\n");
	delay(100);
}

void Ping::setSpeedOfSound(float speed) {
	c = speed;
}

void Ping::read() {
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

			memcpy(&new_sonar_report, &input_buffer, sizeof(new_sonar_report));

			Serial.print("Depth: ");
			Serial.print(new_sonar_report.smoothed_depth_mm);
			Serial.print("mm");
		}
		else
		{
			//Invalid start sequence
			//Move byte to the first index
			test_1 = test_2;
		}
		Serial.write("\n");
	}

}

void Ping::request() {
	stream->write("@D\n");
}

void Ping::calculate() {

}

float Ping::altitude() {
	return 0.0f;
}

uint8_t Ping::confidence() {
	return 0x00;
}
