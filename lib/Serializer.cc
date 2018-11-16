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

/*void Serialize(const void* d, size_t Size){

	if((writePos + Size) > getSize()){
		data.resize(writePos + Size);				
	}

    for (size_t i = 0; i < Size; ++i) {
      data[writePos + Size - i -1] = static_cast<uint8_t>( *(char *)d >> (8 * i));
    }

    writePos += Size;
}*/


template<typename T> 
void Serializer::Serialize(const T* d, size_t Size){

	if((writePos + Size) > getSize()){
		data.resize(writePos + Size);				
	}

    for (size_t i = 0; i < Size; ++i) {
      	data[writePos + Size - i -1] = static_cast<uint8_t>( *(char *)d >> (8 * i));
    }

    writePos += Size;
}

template<typename T> 
void Serializer::Serialize(const T* d){
	Serialize(d, sizeof(*d));
}

template<typename T> 
void Serializer::Serialize(std::vector<T> tab){
	for (size_t i = 0; i < tab.size(); ++i){
		Serialize(&tab[i]);
	}
}

template<typename T> 
void Serializer::Serialize(std::vector<T> tab, size_t Size){
	for (size_t i = 0; i < tab.size(); ++i){
		Serialize(&tab[i], Size);
	}
}


template<typename T> 
T Serializer::Deserialize(T type, size_t Size){
	T x;
	for (size_t i = 0; i < Size; ++i){
		x = data[readPos+i] << (8*(Size-i-1));
	}
	readPos += Size;
	return x;
}
