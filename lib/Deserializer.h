#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include <iostream> 
#include <vector>
#include <cstring>
#include <gf/VectorOps.h>
#include "Serial.h"

class Deserializer{

	private:
		std::vector<uint8_t> data;
		size_t readPos;
		size_t writePos;

		template <typename T>
		void deserializeAnyType(T & d);

	public:
		Deserializer();

		Deserializer(std::vector<uint8_t> & d);

		void printData();

		size_t getSize();

		void clear();

		void setData(std::vector<uint8_t> & d);

		void reset(std::vector<uint8_t> & d);

		void deserialize(uint8_t & d);

		void deserialize(uint16_t & d);

		void deserialize(uint32_t & d);

		void deserialize(uint64_t & d);

		void deserialize(uint8_t *d, size_t Size);

		void deserialize(const gf::Vector2i v);

		void deserialize(Tetromino *t);
};

#endif // DESERIALIZER_H
