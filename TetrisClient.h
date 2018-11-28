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
		static void serverListener(tcp::socket *socketServer, gf::Queue<Message> *queueServer);

};