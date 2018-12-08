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

	void Serializer::serialize(const uint8_t d){

		if((writePos + 2) > getSize()){
			data.resize((writePos+1)*2);				
		}

	    data[writePos] = d;
	  
	    writePos ++;
	}

	void Serializer::serialize(const uint8_t *d, size_t Size){

		if((writePos + Size) > getSize()){
			data.resize((writePos+Size)*2);				
		}

		for (size_t i = 0; i < Size; ++i) {
	      	data[writePos + Size - i -1] = d[i];
	    }
	  
	    writePos += Size;
	}

	void Serializer::serialize(const Tetromino t){
		serialize(t.getType());
		serialize(t.getRotation());
	}

	std::vector<uint8_t> Serializer::getData(){
		size_t size = data.capacity()-sizeof(size_t);
		for (size_t i = 0; i < sizeof(size_t); ++i) {
	    	data[sizeof(size_t) - i - 1] = static_cast<uint8_t>(size >> (8 * i));
	    }
		return data;
	}

	void Serializer::append(char* d, size_t Size){
		if((writePos + Size) > getSize()){
			data.resize(writePos+Size);				
		}
		std::memcpy(&data[writePos], d, Size);
		writePos += Size;
	};

	void Serializer::append(char* d){
		append(d, std::strlen(d));
	};



