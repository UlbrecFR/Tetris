#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>
#include <boost/asio.hpp>
#include <gf/Queue.h>
#include "lib/Serial.h"

using boost::asio::ip::tcp;

class TetrisServer{
	public:
		static void clientListener(tcp::socket *socketClient, gf::Queue<Message> *queueClient);

};

