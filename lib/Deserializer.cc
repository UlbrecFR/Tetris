#include "Deserializer.h"

	Deserializer::Deserializer(){
		readPos = 0;
	};

	Deserializer::Deserializer(std::vector<uint8_t> & d){
		data = d;
		readPos = 0;
	};

	void Deserializer::printData(){
		for (size_t i = 0; i < data.size(); ++i){
			std::cout << (unsigned)data[i] << '.';
		}
		std::cout << std::endl;
	}

	size_t Deserializer::getSize(){
		return data.size();
	}


	void Deserializer::clear(){
		data.clear();
		readPos = 0;
	}


	void Deserializer::setData(std::vector<uint8_t> & d){
		data = d;
	}

	void Deserializer::reset(std::vector<uint8_t> & d){
		clear();
		setData(d);
	}


	template <typename T>
	void Deserializer::deserializeAnyType(T & d){
		T res = 0;
		for (size_t i = 0; i < sizeof(T); ++i) {
	    	res = (res << 8) + data[readPos+sizeof(T)-1-i];
	    }
	    readPos += sizeof(T);
	    d = res;
	}

	void Deserializer::deserialize(uint8_t & d){
	   	deserializeAnyType(d);
	}

	void Deserializer::deserialize(uint16_t & d){
	   	deserializeAnyType(d);
	}

	void Deserializer::deserialize(uint32_t & d){
		deserializeAnyType(d);
	}

	void Deserializer::deserialize(uint64_t & d){
		deserializeAnyType(d);
	}

	/* void Deserializer::deserialize(uint8_t *d, size_t Size){
		for (size_t i = 0; i < Size; ++i){
			d[i] = (uint8_t)data[readPos + Size - i -1];
		}
		readPos += Size*sizeof(d[0]);
	}*/

	void Deserializer::deserialize(gf::Vector2u & v){
		deserialize(v.x);
		deserialize(v.y);
	}

	void Deserializer::deserialize(Tetromino & t){
		gf::Vector2u v;
		deserialize(v);
		t.setX(v.x);
		t.setY(v.y);
		t.setType(data[readPos]);
		t.setRotation(data[readPos+1]);
		readPos += 2;
	}
/*
	void Deserializer::deserialize(gf::Array2D<uint8_t, uint8_t> & array){
		uint8_t witdh, height;
		deserialize(witdh);
		deserialize(height);
		array = gf::Array2D<uint8_t, uint8_t>(gf::Vector2u(witdh, height));
		for(auto& x : array){
			deserialize(x);
		}
	}
*/
	void Deserializer::deserialize(STC_GameStart & r){
		deserialize(r.firstTetro);
		deserialize(r.secondTetro);
	}
		/*
	void Deserializer::deserialize(STC_UpdateOtherPlayer & r){
		deserialize(r.grid);
	}
*/
	void Deserializer::deserialize(STC_NewTetromino & r){
		deserialize(r.newTetro);
	}

	void Deserializer::deserialize(Request_STC::Type & t){
		deserialize(t);
	}

	void Deserializer::deserialize(Request_STC & r){
		uint8_t t;
		deserialize(t);
		r.type = (Request_STC::Type)t;

		switch (r.type) {
			case Request_STC::TYPE_NEW_TETROMINO :
				deserialize(r.newTetroMsg);
				break;
			/*case Request_STC::TYPE_UPDATE_OTHER :
				deserialize(r.updateOtherMsg);
				break;
*/			case Request_STC::TYPE_GAME_START :
				deserialize(r.gameStart);
				break;
		}
	}

	void Deserializer::deserialize(CTS_TetrominoPlaced & r){
		deserialize(r.tetro);
	}
	
	void Deserializer::deserialize(CTS_GameOver & r){
		deserialize(r.tetro);
	}

	void Deserializer::deserialize(CTS_ClientConnectionLost::ErrorType & t){
		deserialize(t);
	}

	void Deserializer::deserialize(CTS_ClientConnectionLost & r){
		deserialize(r.error);
	}

	void Deserializer::deserialize(Request_CTS::Type & t){
		deserialize(t);
	}

	void Deserializer::deserialize(Request_CTS & r){
		uint8_t t;
		deserialize(t);
		r.type = (Request_CTS::Type)t;

		switch (r.type) {
			case Request_CTS::TYPE_TETROMINO_PLACED :
				deserialize(r.tetroMsg);
				break;
			case Request_CTS::TYPE_GAME_OVER :
				deserialize(r.gameOverMsg);
				break;
			case Request_CTS::TYPE_CLIENT_CONNECTION_LOST :
				deserialize(r.discoMsg);
				break;
		}
	}

