#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <gf/Queue.h>
#include <thread>
#include "lib/Serial.h"

using boost::asio::ip::tcp;

class TetrisClient{
	public:
		static void server_listener(tcp::socket *socket_server, gf::Queue<Message> *queue_server);

};