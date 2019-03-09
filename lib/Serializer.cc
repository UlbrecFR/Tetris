#include "Serializer.h"

	Serializer::Serializer(){
		writePos = sizeof(uint64_t);
		data.resize(sizeof(uint64_t));
	};

	void Serializer::printData(){
		for (size_t i = 0; i < writePos; ++i){
			std::cout << (unsigned)data[i] << '.';
		}
		std::cout << std::endl;
	}

	size_t Serializer::getSize(){
		return data.size();
	}

	void Serializer::clear(){
		data.clear();
		writePos = sizeof(uint64_t);
		data.resize(sizeof(uint64_t));
	}

	template <typename T>
	void Serializer::serializeAnyType(T d){
		size_t size = sizeof(T);
		for (size_t i = 0; i < size; ++i) {
	    	data.push_back(static_cast<uint8_t>(d >> 8*(size - i - 1))); 
	    }
	    writePos += sizeof(T);
	}


	void Serializer::serialize(const uint8_t d){
		serializeAnyType(d);
	}

	void Serializer::serialize(const uint16_t d){
		serializeAnyType(d);
	}

	void Serializer::serialize(const uint32_t d){
		serializeAnyType(d);
	}

	void Serializer::serialize(const uint64_t d){
		serializeAnyType(d);
	}

	template <typename T>
	void Serializer::serialize(const T *d, uint64_t Size){
		serialize(Size);

		for (size_t i = 0; i < Size; ++i) {
	      	serialize(d[i]);
	    }
	}

	void Serializer::serialize(const Grid g){
		for (size_t row = 0; row < g.getRows(); ++row){
	        for (size_t col = 0; col < g.getCols(); ++col){
	            serialize(g(col,row));
	        }
	    }
	}

	void Serializer::serialize(const gf::Vector2u v){
		serialize(v.x);
		serialize(v.y);
	}

	void Serializer::serialize(const Tetromino t){
		serialize(t.getPos());
		serialize(t.getType());
		serialize(t.getRotation());
	}

	void Serializer::serialize(const STC_MalusStart r){
		serialize(r.typeMalus);
		serialize(r.target);
	}

	void Serializer::serialize(const STC_MalusEnd r){
		serialize(r.target);
	}

	void Serializer::serialize(const STC_GameStart r){
		serialize(r.firstTetro);
		serialize(r.secondTetro);
		serialize(r.time);
	}

	void Serializer::serialize(const STC_UpdateGrid r){
		serialize(r.grid);
		serialize(r.score);
	}
	
	void Serializer::serialize(const STC_UpdateOtherGrid r){
		serialize(r.grid);
		serialize(r.score);
	}

	void Serializer::serialize(const STC_NewTetromino r){
		serialize(r.newTetro);
	}

	void Serializer::serialize(const STC_GameOver::Results r){
		serialize((uint8_t)r);
	}

	void Serializer::serialize(const STC_GameOver r){
		serialize(r.results);
	}

	void Serializer::serialize(const Request_STC::Type t){
		serialize((uint8_t)t);
	}

	void Serializer::serialize(const Request_STC r){
		serialize(r.type);

		switch (r.type) {
			case Request_STC::TYPE_NEW_TETROMINO :
				serialize(r.newTetroMsg);
				break;
			case Request_STC::TYPE_UPDATE :
				serialize(r.updateMsg);
				break;
			case Request_STC::TYPE_UPDATE_OTHER :
				serialize(r.updateOtherMsg);
				break;
			case Request_STC::TYPE_GAME_START :
				serialize(r.gameStart);
				break;
			case Request_STC::TYPE_GAME_OVER :
				serialize(r.gameOver);
				break;
			case Request_STC::TYPE_MALUS_START :
				serialize(r.malusStart);
				break;			
			case Request_STC::TYPE_MALUS_END :
				serialize(r.malusEnd);
				break;
		}
	}

	void Serializer::serialize(const CTS_TetrominoPlaced r){
		serialize(r.tetro);
	}

	void Serializer::serialize(const CTS_ClientConnectionLost::ErrorType t){
		serialize((uint8_t)t);
	}
	
	void Serializer::serialize(const CTS_ClientConnectionLost r){
		serialize(r.error);
	}

	void Serializer::serialize(const Request_CTS::Type t){
		serialize((uint8_t)t);
	}

	void Serializer::serialize(const Request_CTS r){
		serialize(r.type);

		switch (r.type) {
			case Request_CTS::TYPE_TETROMINO_PLACED :
				serialize(r.tetroMsg);
				break;
			case Request_CTS::TYPE_CLIENT_CONNECTION_LOST :
				serialize(r.discoMsg);
				break;
		}
	}

	std::vector<uint8_t> Serializer::getData(){
		size_t size = data.size()-sizeof(uint64_t);
		for (size_t i = 0; i < sizeof(uint64_t); ++i) {
	    	data[i] = static_cast<uint8_t>(size >> (8 * (sizeof(uint64_t)-i-1)));
	    }
		return data;
	}



