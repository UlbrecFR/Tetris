
#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <iostream> 
#include <vector>
#include <cstring>
#include <gf/VectorOps.h>
#include <gf/Array2D.h>
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

		//void serialize(const uint8_t *d, size_t Size);

		void serialize(const gf::Vector2u v);

		void serialize(const Tetromino t);

		void serialize(const gf::Array2D<uint8_t, uint8_t> array);

		void serialize(const STC_GameStart r);
		
		//void serialize(const STC_UpdateOtherPlayer r);

		void serialize(const STC_NewTetromino r);

		void serialize(const CTS_GameOver r);

		void serialize(const Request_STC::Type t);

		void serialize(const Request_STC r);

		void serialize(const CTS_TetrominoPlaced r);

		void serialize(const CTS_ClientConnectionLost::ErrorType t);
	
		void serialize(const CTS_ClientConnectionLost r);

		void serialize(const Request_CTS::Type t);

		void serialize(const Request_CTS r);

		std::vector<uint8_t> getData();
};

#endif // SERIALIZER_H