#include "Ping.h"

Ping::Ping(Stream *_stream) {
	stream = _stream;
	c = 1400;
}

void Ping::init() {

}

void Ping::setSpeedOfSound(float speed) {
	c = speed;
}

void Ping::read() {
	while (stream->available() > MIN_PACKET_LENGTH) {

	}

	calculate();
}

void Ping::calculate() {

}

float Ping::altitude() {
	return 0.0f;
}

uint8_t Ping::confidence() {
	return 0x00;
}