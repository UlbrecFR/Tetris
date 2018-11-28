#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>
#include <boost/asio.hpp>
#include <gf/Queue.h>

#include "TetrisServer.h"

using boost::asio::ip::tcp;

static void client_listener(tcp::socket *socket_client, gf::Queue<Message> *queue_client) {

    for(;;) {

        //on lit le message envoyé par le client
        Message msg;

        boost::system::error_code error;
        size_t length = socket_client->read_some(boost::asio::buffer(msg.msg), error);

        msg.length = length;
        //on gère les erreurs
        if (error == boost::asio::error::eof)
            break; // Connection closed cleanly by peer.
        else if (error)
            throw boost::system::system_error(error); // Some other error.

        queue_client->push(msg);
    } 
}

int main(int argc, char* argv[]){
	try {
       	if (argc != 2) {
            std::cerr << "Usage: blocking_tcp_echo_server <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;
        tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), std::atoi(argv[1])));

        tcp::socket *sock1(new tcp::socket(io_service));
        tcp::socket *sock2(new tcp::socket(io_service));

        a.accept(*sock1);
        a.accept(*sock2);

        gf::Queue<Message> queueCli1;
        gf::Queue<Message> queueCli2;

        std::thread(client_listener, sock1, &queueCli1).detach();
        std::thread(client_listener, sock2, &queueCli2).detach();

        Message voui;

        for(;;) {
            if (queueCli1.poll(voui)) {
                boost::asio::write(*sock2, boost::asio::buffer(voui.msg, voui.length));
            }

            if (queueCli2.poll(voui)) {
                boost::asio::write(*sock1, boost::asio::buffer(voui.msg, voui.length));
            }

        }

    } catch (std::exception& e) {
        std::cerr << "Exception SERVER: " << e.what() << "\n";
    }
    


	return 0;

}