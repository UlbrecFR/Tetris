#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <gf/Queue.h>
#include <thread>
#include "TetrisClient.h"

using boost::asio::ip::tcp;


static void server_listener(tcp::socket *socket_server, gf::Queue<Message> *queue_server) {

    for(;;) {

        Message msg;

        boost::system::error_code error;
        size_t length = socket_server->read_some(boost::asio::buffer(msg.msg), error);

        msg.length = length;
        //on gère les erreurs
        if (error == boost::asio::error::eof)
            break; // Connection closed cleanly by peer.
        else if (error)
            throw boost::system::system_error(error); // Some other error.

        queue_server->push(msg);


    }

}

int main(int argc, char* argv[]){

    try {
        if (argc != 3) {
            std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        tcp::socket *sock(new tcp::socket(io_service));
        tcp::resolver resolver(io_service);
        boost::asio::connect(*sock, resolver.resolve({argv[1], argv[2]}));

        gf::Queue<Message> queue_server;

        std::thread(server_listener, sock, &queue_server).detach();
    
        for(;;) {
            char request[max_length];
            std::cin.getline(request, max_length);
            size_t request_length = std::strlen(request);
            boost::asio::write(*sock, boost::asio::buffer(request, request_length));
        
            static Message voui;

            if (queue_server.poll(voui)) {
                std::cout << voui.msg << std::endl;
            }
        }


    } catch (std::exception& e) {
        std::cerr << "Exception CLIENT : " << e.what() << "\n";
    }

    return 0;
}
