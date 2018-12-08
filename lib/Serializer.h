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

		size_t getReadPos();

		size_t getWritePos();

		size_t getSize();

		void setReadPos(size_t pos);

		void setWritePos(size_t pos);

		void clear();

		void Serialize(const uint8_t d);

		void Serialize(const uint8_t *d, size_t Size);

		void Serialize(const Tetromino t);

		const void *getData();

		void append(char* d, size_t Size);

		void append(char* d);

		void Deserialize(uint8_t &d);

		void Deserialize(uint8_t *d, size_t Size);

		void Deserialize(Tetromino *t);
};

#endif // SERIALIZER_H