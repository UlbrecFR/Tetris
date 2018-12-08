#include "Serializer.h"

	Serializer::Serializer(){
		readPos = 0;
		writePos = 0;
	};

	void Serializer::printData(){
		for (size_t i = 0; i < writePos; ++i){
			std::cout << (unsigned)data[i] << '.';
		}
		std::cout << std::endl;
	}

	size_t Serializer::getReadPos(){
		return readPos;
	}

	size_t Serializer::getWritePos(){
		return writePos;
	}

	size_t Serializer::getSize(){
		return data.size();
	}

	void Serializer::setReadPos(size_t pos){
		if(data.size()>pos){readPos = pos;}
	}

	void Serializer::setWritePos(size_t pos){
		if(data.size()>pos){writePos = pos;}
	}

	void Serializer::clear(){
		data.clear();
		setReadPos(0);
		setWritePos(0);
	}

	void Serializer::Serialize(const uint8_t d){

		if((writePos + 2) > getSize()){
			data.resize((writePos+1)*2);				
		}

	    data[writePos] = d;
	  
	    writePos ++;
	}

	void Serializer::Serialize(const uint8_t *d, size_t Size){

		if((writePos + Size) > getSize()){
			data.resize((writePos+Size)*2);				
		}

		for (size_t i = 0; i < Size; ++i) {
	      	data[writePos + Size - i -1] = d[i];
	    }
	  
	    writePos += Size;
	}

	void Serializer::Serialize(const Tetromino t){
		Serialize(t.getType());
		Serialize(t.getRotation());
	}

	const void *Serializer::getData(){
		return &data[0];
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

	void Serializer::Deserialize(uint8_t &d){
	    d = (uint8_t)data[readPos];
	    readPos += sizeof(d);
	}

	void Serializer::Deserialize(uint8_t *d, size_t Size){
		for (size_t i = 0; i < Size; ++i){
			d[i] = (uint8_t)data[readPos + Size - i -1];
		}
		readPos += Size*sizeof(d[0]);
	}	

	void Serializer::Deserialize(Tetromino *t){
		t->setType(data[readPos]);
		t->setRotation(data[readPos+1]);
		readPos += 2;
	}


