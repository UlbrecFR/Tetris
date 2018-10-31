#include <cstdio>
#include <cstdlib>
#include <cstring>

class Serial {
	public:
		enum { header_length = 4 };
		enum { max_body_length = 512 };

		Serial(): body_length_(0) {
		}


		const char* data() const;

		char* data();

		std::size_t length() const;

		char* body();

		std::size_t body_length() const;

		void body_length(std::size_t new_length);

		bool decode_header();

		void encode_header();

	private:

		char data_[header_length + max_body_length];
		std::size_t body_length_;


};
