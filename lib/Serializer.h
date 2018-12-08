#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <iostream> 
#include <vector>
#include <cstring>
#include "Serial.h"

class Serializer{

	private:
		std::vector<uint8_t> data;
		size_t readPos;
		size_t writePos;


	public:
		Serializer();

		void printData();

		size_t getSize();

		void clear();

		void serialize(const uint8_t d);

		void serialize(const uint8_t *d, size_t Size);

		void serialize(const Tetromino t);

		std::vector<uint8_t> getData();

		void append(char* d, size_t Size);

		void append(char* d);
};

#endif // SERIALIZER_H