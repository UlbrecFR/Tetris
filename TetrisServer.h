#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>
#include <time.h>
#include <random>

#include <boost/asio.hpp>

#include <gf/Queue.h>
#include <gf/Clock.h>

#include <Serial.h>
#include <Serializer.h>
#include <Deserializer.h>
#include <Tetromino.h>
#include <Serial.h>
#include <Grid.h>

#define NB_PLAYERS 2

using boost::asio::ip::tcp;

class TetrisServer{
	public:
		static void clientListener(tcp::socket *socketClient, gf::Queue<std::vector<uint8_t>> *queueClient);

};

