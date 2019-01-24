#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>
#include <boost/asio.hpp>
#include <gf/Queue.h>
#include <Serial.h>
#include <Serializer.h>
#include <Deserializer.h>
#include <Tetromino.h>
#include <Serial.h>
#include <random>

using boost::asio::ip::tcp;

class TetrisServer{
	public:
		static void clientListener(tcp::socket *socketClient, gf::Queue<std::vector<uint8_t>> *queueClient);

};

