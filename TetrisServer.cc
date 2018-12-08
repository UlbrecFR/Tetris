#include "TetrisServer.h"

using boost::asio::ip::tcp;

static void clientListener(tcp::socket *socketClient, gf::Queue<std::vector<uint8_t>> *queueClient) {

    for(;;) {

        //on lit le message envoyé par le client
        std::vector<uint8_t> length(sizeof(size_t));

        boost::system::error_code error;
        socketClient->read_some(boost::asio::buffer(length), error);

        Deserializer ds(length);

        size_t size;
        ds.deserialize(size);

        std::vector<uint8_t> msg(size);

        socketClient->read_some(boost::asio::buffer(msg), error);

        //on gère les erreurs
        if (error == boost::asio::error::eof)
            break; // Connection closed cleanly by peer.
        else if (error)
            throw boost::system::system_error(error); // Some other error.

        queueClient->push(msg);
    } 
}

int main(int argc, char* argv[]){

    printf("%s\n", "Tetris Server : Le Vouitris");

	try {
       	if (argc != 2) {
            std::cerr << "Usage: blocking_tcp_echo_server <port>\n";
            return 1;
        }

        boost::asio::io_service ioService;
        tcp::acceptor a(ioService, tcp::endpoint(tcp::v4(), std::atoi(argv[1])));

        tcp::socket *sock1(new tcp::socket(ioService));
        tcp::socket *sock2(new tcp::socket(ioService));

        a.accept(*sock1);
        a.accept(*sock2);

        gf::Queue<std::vector<uint8_t>> queueCli1;
        gf::Queue<std::vector<uint8_t>> queueCli2;

        std::thread(clientListener, sock1, &queueCli1).detach();
        std::thread(clientListener, sock2, &queueCli2).detach();

        std::vector<uint8_t> msg;

        for(;;) {
            if (queueCli1.poll(msg)) {

                Deserializer ds(msg);
               
                Tetromino t;
                ds.deserialize(&t);

                Serializer s;
                s.serialize(t);

                std::vector<uint8_t> request;
                request = s.getData();

                boost::asio::write(*sock2, boost::asio::buffer(request));
            }

            if (queueCli2.poll(msg)) {
                Deserializer ds(msg);
               
                Tetromino t;
                ds.deserialize(&t);

                Serializer s;
                s.serialize(t);

                std::vector<uint8_t> request;
                request = s.getData();

                boost::asio::write(*sock1, boost::asio::buffer(request));
            }

        }

    } catch (std::exception& e) {
        std::cerr << "Exception SERVER: " << e.what() << "\n";
    }
    


	return 0;

}