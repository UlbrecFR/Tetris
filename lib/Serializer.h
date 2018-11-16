#include <iostream> 
#include <vector>

typedef uint8_t Int;

class Serializer{

	private:
		std::vector<Int> data;
		size_t readPos;
		size_t writePos;


	public:
		Serializer();

		void printData();

		size_t getReadPos();

		size_t getWritePos();

		size_t getSize();

		void setReadPos(size_t pos);

		void setWritePos(size_t pos);

		void clear();

		//void Serialize(const void* d, size_t Size);

		template<typename T> 
		void Serialize(const T* d, size_t Size);

		template<typename T> 
		void Serialize(const T* d);

		template<typename T> 
		void Serialize(std::vector<T> tab);

		template<typename T> 
		void Serialize(std::vector<T> tab, size_t Size);

		template<typename T> 
		T Deserialize(T type, size_t Size);
				
};