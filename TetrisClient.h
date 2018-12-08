#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <gf/Queue.h>
#include <thread>
#include <Serial.h>
#include <Serializer.h>
#include <Deserializer.h>
#include <Tetromino.h>

using boost::asio::ip::tcp;

class TetrisClient{
	public:
		static void serverListener(tcp::socket *socketServer, gf::Queue<std::vector<uint8_t>> *queueServer);
};