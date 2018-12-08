#include "TetrisClient.h"

using boost::asio::ip::tcp;


static void serverListener(tcp::socket *socketServer, gf::Queue<std::vector<uint8_t>> *queueServer) {

    for(;;) {

        std::vector<uint8_t> length(sizeof(size_t));

        boost::system::error_code error;
        socketServer->read_some(boost::asio::buffer(length), error);

        Deserializer ds(length);
        size_t size;
        ds.deserialize(size);

        std::vector<uint8_t> msg(size);

        socketServer->read_some(boost::asio::buffer(msg), error);

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

        gf::Queue<std::vector<uint8_t>> queueServer;

        std::thread(serverListener, sock, &queueServer).detach();
    
        for(;;) {
            std::cin.getline(NULL, 0);
            
            Tetromino t;
            t.setRotation(2);
            t.setType(4);

            Serializer s;
            s.serialize(t);

            std::vector<uint8_t> request;
            request = s.getData();
            
            boost::asio::write(*sock, boost::asio::buffer(request, request.capacity()));
        
            std::vector<uint8_t> msg;

            if (queueServer.poll(msg)) {

                Deserializer s(msg);
               
                Tetromino t;
                s.deserialize(&t);
                printf("%d\n", t.getType());
                printf("%d\n", t.getRotation());
            }
        }


    } catch (std::exception& e) {
        std::cerr << "Exception CLIENT : " << e.what() << "\n";
    }

    return 0;
}
