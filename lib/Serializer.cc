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

	void Serializer::serialize(const uint8_t *d, size_t Size){

		if((writePos + Size) > getSize()){
			data.resize((writePos+Size)*2);				
		}

		for (size_t i = 0; i < Size; ++i) {
	      	data[writePos + Size - i -1] = d[i];
	    }
	  
	    writePos += Size;
	}

	void Serializer::serialize(const gf::Vector2u v){
		serialize(v.x);
		serialize(v.y);
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



