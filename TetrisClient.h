#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "lib/Serial.h"

using boost::asio::ip::tcp;

typedef std::deque<Serial> Serial_queue;

class TetrisClient{
	public:
		TetrisClient(boost::asio::io_service& io_service, tcp::resolver::iterator endpoint_iterator)
			: io_service_(io_service), socket_(io_service){
				do_connect(endpoint_iterator);
			}

		void write(const Serial& msg);

		void close();

	private:
		void do_connect(tcp::resolver::iterator endpoint_iterator);

		void do_read_header();

		void do_read_body();

		void do_write();

	private:
		boost::asio::io_service& io_service_;
		tcp::socket socket_;
		Serial read_msg_;
		Serial_queue write_msgs_;

};