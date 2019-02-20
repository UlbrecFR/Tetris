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
		uint64_t readPos;

		template <typename T>
		void deserializeAnyType(T & d);

	public:
		Deserializer();

		Deserializer(std::vector<uint8_t> & d);

		void printData();

		uint64_t getSize();

		void clear();

		void setData(std::vector<uint8_t> & d);

		void reset(std::vector<uint8_t> & d);

		void deserialize(uint8_t & d);

		void deserialize(uint16_t & d);

		void deserialize(uint32_t & d);

		void deserialize(uint64_t & d);

		template <typename T>
		void deserialize(T *d, uint64_t & Size);

		void deserialize(Grid & g);

		void deserialize(gf::Vector2u & v);

		void deserialize(Tetromino & t);

		void deserialize(STC_GameStart & r);

		void deserialize(STC_UpdateGrid & r);
		
		void deserialize(STC_UpdateOtherGrid & r);

		void deserialize(STC_NewTetromino & r);

		void deserialize(STC_MalusStart & r);

		void deserialize(STC_MalusEnd & r);

		void deserialize(STC_GameOver & r);
		
		void deserialize(Request_STC::Type & t);

		void deserialize(Request_STC & r);

		void deserialize(CTS_TetrominoPlaced & r);
	
		void deserialize(CTS_ClientConnectionLost & r);

		void deserialize(Request_CTS::Type & t);

		void deserialize(Request_CTS & r);
};

#endif // DESERIALIZER_H
