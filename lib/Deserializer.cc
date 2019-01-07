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

	void Deserializer::deserialize(uint8_t *d, size_t Size){
		for (size_t i = 0; i < Size; ++i){
			d[i] = (uint8_t)data[readPos + Size - i -1];
		}
		readPos += Size*sizeof(d[0]);
	}

	void Deserializer::deserialize(gf::Vector2u *v){
		deserialize(v->x);
		deserialize(v->y);
	}

	void Deserializer::deserialize(Tetromino *t){
		gf::Vector2u v;
		deserialize(&v);
		t->setX(v.x);
		t->setY(v.y);
		t->setType(data[readPos]);
		t->setRotation(data[readPos+1]);
		readPos += 2;
	}