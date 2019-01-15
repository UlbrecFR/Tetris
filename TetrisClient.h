#include <cstdlib>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include <boost/asio.hpp>

#include <gf/Queue.h>
#include <gf/VectorOps.h>
#include <gf/Array2D.h>
#include <gf/Vector.h>

#include <gf/RenderWindow.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>
#include <gf/Action.h>
#include <gf/Sprite.h>
#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/EntityContainer.h>
#include <gf/Event.h>

#include <Serial.h>
#include <Serializer.h>
#include <Deserializer.h>
#include <Tetromino.h>

using boost::asio::ip::tcp;


class TetrisClient{
	public:
		static void serverListener(tcp::socket *socketServer, gf::Queue<std::vector<uint8_t>> *queueServer);
};