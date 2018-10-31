#include "Serial.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

const char* Serial::data() const {
	return data_;
}

char* Serial::data() {
	return data_;
}

std::size_t Serial::length() const {
	return header_length + body_length_;
}

char* Serial::body() {
	return data_ + header_length;
}

std::size_t Serial::body_length() const {
	return body_length_;
}

void Serial::body_length(std::size_t new_length) {
	body_length_ = new_length;
	if (body_length_ > max_body_length)
		body_length_ = max_body_length;
}

bool Serial::decode_header() {
	char header[header_length + 1] = "";
	std::strncat(header, data_, header_length);
	body_length_ = std::atoi(header);

	if (body_length_ > max_body_length) {
		body_length_ = 0;
		return false;
	}
	
	return true;
}

void Serial::encode_header() {
	char header[header_length + 1] = "";
	std::sprintf(header, "%4d", body_length_);
	std::memcpy(data_, header, header_length);
}