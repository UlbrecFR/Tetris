#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <gf/Queue.h>
#include <thread>
#include "TetrisClient.h"

using boost::asio::ip::tcp;


static void serverListener(tcp::socket *socketServer, gf::Queue<Message> *queueServer) {

    for(;;) {

        Message msg;

        boost::system::error_code error;
        size_t length = socketServer->read_some(boost::asio::buffer(msg.msg), error);

        msg.length = length;
        //on gère les erreurs
        if (error == boost::asio::error::eof)
            break; // Connection closed cleanly by peer.
        else if (error)
            throw boost::system::system_error(error); // Some other error.

        queueServer->push(msg);


    }

}

int main(int argc, char* argv[]){

    printf("%s\n", "Tetris Client : Le Vouitris");

    try {
        if (argc != 3) {
            std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        tcp::socket *sock(new tcp::socket(io_service));
        tcp::resolver resolver(io_service);
        boost::asio::connect(*sock, resolver.resolve({argv[1], argv[2]}));

        gf::Queue<Message> queueServer;

        std::thread(serverListener, sock, &queueServer).detach();
    
        for(;;) {
            char request[maxLength];
            std::cin.getline(request, maxLength);
            size_t requestLength = std::strlen(request);
            boost::asio::write(*sock, boost::asio::buffer(request, requestLength));
        
            Message msg;

            if (queueServer.poll(msg)) {
                std::cout << msg.msg << std::endl;
            }
        }


    } catch (std::exception& e) {
        std::cerr << "Exception CLIENT : " << e.what() << "\n";
    }

    return 0;
}
