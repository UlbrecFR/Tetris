#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>
#include "lib/Serial.h"

using boost::asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<Serial> Serial_queue;

//----------------------------------------------------------------------

class Player {
	public:
		virtual ~Player() {}
		virtual void deliver(const Serial& msg) = 0;
};

typedef std::shared_ptr<Player> Player_ptr;

//----------------------------------------------------------------------

class Lobby {
	public:
		void join(Player_ptr participant);

	 	void leave(Player_ptr participant);

		void deliver(const Serial& msg);

	private:
		std::set<Player_ptr> players_;
		enum { max_recent_msgs = 100 };
		Serial_queue recent_msgs_;
};

//----------------------------------------------------------------------

class GameSession: 
	public Player,
	public std::enable_shared_from_this<GameSession>{
	public:
		GameSession(tcp::socket socket, Lobby& room)
			: socket_(std::move(socket)),room_(room){}

		void start();

		void deliver(const Serial& msg);

	private:
		void do_read_header();

		void do_read_body();

		void do_write();

		tcp::socket socket_;
		Lobby& room_;
		Serial read_msg_;
		Serial_queue write_msgs_;
};

//----------------------------------------------------------------------

class TetrisServer{
  
	public:
		TetrisServer(boost::asio::io_service& io_service,
			const tcp::endpoint& endpoint) : acceptor_(io_service, endpoint),
			socket_(io_service){
				do_accept();
			}

	private:
		void do_accept();

		tcp::acceptor acceptor_;
		tcp::socket socket_;
		Lobby room_;
};

//----------------------------------------------------------------------

