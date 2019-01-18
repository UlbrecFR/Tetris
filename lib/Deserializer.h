#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include <iostream> 
#include <vector>
#include <cstring>
#include <gf/VectorOps.h>
#include <gf/Array2D.h>
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

		//void deserialize(uint8_t *d, size_t Size);

		void deserialize(gf::Vector2u & v);

		void deserialize(Tetromino & t);

		void deserialize(gf::Array2D<uint8_t, uint8_t> & array);

		void deserialize(STC_GameStart & r);
		
		//void deserialize(STC_UpdateOtherPlayer & r);

		void deserialize(STC_NewTetromino & r);

		void deserialize(Request_STC::Type & t);

		void deserialize(Request_STC & r);

		void deserialize(CTS_TetrominoPlaced & r);

		void deserialize(CTS_GameOver & r);

		void deserialize(CTS_ClientConnectionLost::ErrorType & t);
	
		void deserialize(CTS_ClientConnectionLost & r);

		void deserialize(Request_CTS::Type & t);

		void deserialize(Request_CTS & r);
};

#endif // DESERIALIZER_H
