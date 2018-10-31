#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>
#include "TetrisServer.h"

using boost::asio::ip::tcp;

//----------------------------------------------------------------------
	void Lobby::join(Player_ptr player) {
		players_.insert(player);
		for (auto msg: recent_msgs_) player->deliver(msg);
	}

	void Lobby::leave(Player_ptr player) {
		players_.erase(player);
	}

	void Lobby::deliver(const Serial& msg) {
		recent_msgs_.push_back(msg);
		while (recent_msgs_.size() > max_recent_msgs)
			recent_msgs_.pop_front();

		for (auto player: players_)
			player->deliver(msg);
	}



//----------------------------------------------------------------------


	void GameSession::start() {
		room_.join(shared_from_this());
		do_read_header();
	}

	void GameSession::deliver(const Serial& msg) {

		bool write_in_progress = !write_msgs_.empty();
		write_msgs_.push_back(msg);

		if (!write_in_progress) {
			do_write();
		}
	}

	void GameSession::do_read_header() {
		auto self(shared_from_this());
		boost::asio::async_read(socket_,
		boost::asio::buffer(read_msg_.data(), read_msg_.header_length),
			[this, self](boost::system::error_code ec, std::size_t){
			if (!ec && read_msg_.decode_header()){
				do_read_body();
			}else{
				room_.leave(shared_from_this());
			}
		});
	}

	void GameSession::do_read_body(){
		auto self(shared_from_this());
		boost::asio::async_read(socket_,
		boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
			[this, self](boost::system::error_code ec, std::size_t) {
			if (!ec){
				room_.deliver(read_msg_);
				do_read_header();
			} else {
				room_.leave(shared_from_this());
			}
		});
	}

	void GameSession::do_write(){
		auto self(shared_from_this());
		boost::asio::async_write(socket_,
		boost::asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()),
			[this, self](boost::system::error_code ec, std::size_t /*length*/) {
			if (!ec) {
				write_msgs_.pop_front();
				if (!write_msgs_.empty()){
					do_write();
				}
			}else {
				room_.leave(shared_from_this());
			}
		});
	}


//----------------------------------------------------------------------

	void TetrisServer::do_accept() {
		acceptor_.async_accept(socket_,[this](boost::system::error_code ec){
			if (!ec){
				std::make_shared<GameSession>(std::move(socket_), room_)->start();
			}

			do_accept();
		});
	}

//----------------------------------------------------------------------

int main(int argc, char* argv[]){
	try{
		if (argc < 2){
			std::cerr << "Usage:tetrisServer <port> [<port> ...]\n";
			return 1;
		}

		boost::asio::io_service io_service;

		std::list<TetrisServer> servers;
		for (int i = 1; i < argc; ++i){
			tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
			servers.emplace_back(io_service, endpoint);
		}

		io_service.run();
	} catch (std::exception& e){
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;

}