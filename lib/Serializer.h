
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
		uint64_t writePos;

		template <typename T>
		void serializeAnyType(T d);


	public:
		Serializer();

		void printData();

		uint64_t getSize();

		void clear();

		void serialize(const uint8_t d);

		void serialize(const uint16_t d);

		void serialize(const uint32_t d);

		void serialize(const uint64_t d);

		template <typename T>
		void serialize(const T *d, uint64_t Size);

		void serialize(const Grid g);

		void serialize(const gf::Vector2u v);

		void serialize(const Tetromino t);

		void serialize(const STC_GameStart r);

		void serialize(const STC_UpdateGrid r);
		
		void serialize(const STC_UpdateOtherGrid r);

		void serialize(const STC_NewTetromino r);

		void serialize(const STC_GameOver::Results r);

		void serialize(const STC_GameOver r);

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