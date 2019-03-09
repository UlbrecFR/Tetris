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
	    	res = (res << 8) + data[readPos+i];
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

	template <typename T>
	void Deserializer::deserialize(T *d, uint64_t & Size){

		deserialize(Size);

		for (size_t i = 0; i < Size; ++i){
			deserialize(d[i]);
		}
	}

	void Deserializer::deserialize(Grid & g){
		for (size_t row = 0; row < g.getRows(); ++row){
	        for (size_t col = 0; col < g.getCols(); ++col){
	            deserialize(g(col,row));
	        }
	    }
	}

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

	void Deserializer::deserialize(STC_MalusStart & r){
		deserialize(r.typeMalus);
		deserialize(r.target);
	}

	void Deserializer::deserialize(STC_MalusEnd & r){
		deserialize(r.target);
	}

	void Deserializer::deserialize(STC_GameStart & r){
		deserialize(r.firstTetro);
		deserialize(r.secondTetro);
		deserialize(r.time);
	}

	void Deserializer::deserialize(STC_UpdateGrid & r){
		deserialize(r.grid);
		deserialize(r.score);
	}

	void Deserializer::deserialize(STC_UpdateOtherGrid & r){
		deserialize(r.grid);
		deserialize(r.score);
	}

	void Deserializer::deserialize(STC_NewTetromino & r){
		deserialize(r.newTetro);
	}

	void Deserializer::deserialize(STC_GameOver & r){
		uint8_t t;
		deserialize(t);
		r.results = (STC_GameOver::Results)t;
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
			case Request_STC::TYPE_UPDATE :
				deserialize(r.updateMsg);
				break;
			case Request_STC::TYPE_UPDATE_OTHER :
				deserialize(r.updateOtherMsg);
				break;
			case Request_STC::TYPE_GAME_START :
				deserialize(r.gameStart);
				break;
			case Request_STC::TYPE_GAME_OVER :
				deserialize(r.gameOver);
				break;
			case Request_STC::TYPE_MALUS_START :
				deserialize(r.malusStart);
				break;			
			case Request_STC::TYPE_MALUS_END :
				deserialize(r.malusEnd);
				break;
		}
	}

	void Deserializer::deserialize(CTS_TetrominoPlaced & r){
		deserialize(r.tetro);
	}

	void Deserializer::deserialize(CTS_ClientConnectionLost & r){
		uint8_t t;
		deserialize(t);
		r.error = (CTS_ClientConnectionLost::ErrorType)t;
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
			case Request_CTS::TYPE_CLIENT_CONNECTION_LOST :
				deserialize(r.discoMsg);
				break;
		}
	}

