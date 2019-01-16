#include "Serializer.h"

	Serializer::Serializer(){
		writePos = sizeof(size_t);
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
		writePos = sizeof(size_t);
	}

	template <typename T>
	void Serializer::serializeAnyType(T d){

		if((writePos + 2) > getSize()){
			data.resize((writePos+1)*2);				
		}

		for (size_t i = 0; i < sizeof(T); ++i) {
	    	data[writePos+i] = (d >> 8*i); 
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

	/* void Serializer::serialize(const uint8_t *d, size_t Size){

		if((writePos + Size) > getSize()){
			data.resize((writePos+Size)*2);				
		}

		for (size_t i = 0; i < Size; ++i) {
	      	data[writePos + Size - i -1] = d[i];
	    }
	  
	    writePos += Size;
	}*/

	void Serializer::serialize(const gf::Vector2u v){
		serialize(v.x);
		serialize(v.y);
	}

	void Serializer::serialize(const Tetromino t){
		serialize(t.getPos());
		serialize(t.getType());
		serialize(t.getRotation());
	}

	void Serializer::serialize(const gf::Array2D<uint8_t, uint8_t> array){
		auto size = array.getSize();
		serialize(size.width);
		serialize(size.height);
		for (auto& x : array){
			serialize(x);
		}
	}

	void Serializer::serialize(const STC_GameStart r){
		serialize(r.firstTetro);
	}
	
	void Serializer::serialize(const STC_UpdateOtherPlayer r){
		serialize(r.grid);
	}

	void Serializer::serialize(const STC_NewTetromino r){
		serialize(r.newTetro);
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
			case Request_STC::TYPE_UPDATE_OTHER :
				serialize(r.updateOtherMsg);
				break;
			case Request_STC::TYPE_GAME_START :
				serialize(r.gameStart);
				break;
		}
	}

	void Serializer::serialize(const CTS_TetrominoPlaced r){
		serialize(r.tetro);
	}

	void Serializer::serialize(const CTS_ClientConnectionLost::ErrorType t){
		serialize(t);
	}
	
	void Serializer::serialize(const CTS_ClientConnectionLost r){
		serialize(r.error);
	}

	void Serializer::serialize(const CTS_NextTetrominoPlease r){
		
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
			case Request_CTS::TYPE_CLIENT_LOST :
				serialize(r.discoMsg);
				break;
			case Request_CTS::TYPE_NEXT_TETRO :
				serialize(r.nextTetroMsg);
				break;
		}
	}

	std::vector<uint8_t> Serializer::getData(){
		size_t size = data.capacity()-sizeof(size_t);
		for (size_t i = 0; i < sizeof(size_t); ++i) {
	    	data[i] = static_cast<uint8_t>(size >> (8 * i));
	    }
		return data;
	}



