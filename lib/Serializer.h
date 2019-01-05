#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <iostream> 
#include <vector>
#include <cstring>
#include <gf/VectorOps.h>
#include "Serial.h"

class Serializer{

	private:
		std::vector<uint8_t> data;
		size_t writePos;

		template <typename T>
		void serializeAnyType(T d);


	public:
		Serializer();

		void printData();

		size_t getSize();

		void clear();

		void serialize(const uint8_t d);

		void serialize(const uint16_t d);

		void serialize(const uint32_t d);

		void serialize(const uint64_t d);

		void serialize(const uint8_t *d, size_t Size);

		void serialize(const gf::Vector2u v);

		void serialize(const Tetromino t);

		std::vector<uint8_t> getData();
};

#endif // SERIALIZER_H