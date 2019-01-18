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

        printf("RECU !!\n");

        queueClient->push(msg);
    } 
}

void sendNewTetro(tcp::socket *socketClient, int id) {
        Serializer s;
        std::vector<uint8_t> serializedRequest;

        Tetromino t;

        t.setRotation(0);
        t.setType(0);
        t.setPos({6,1});
        t.setType(rand()%7+1);

        printf("%d type : %d\n", id, t.getType());

        Request_STC rqSTC;
        rqSTC.type = Request_STC::TYPE_NEW_TETROMINO;

        rqSTC.newTetroMsg.newTetro = t;

        s.serialize(rqSTC);
        serializedRequest = s.getData();
        boost::asio::write(*socketClient, boost::asio::buffer(serializedRequest));
        s.clear();

}

void exploitMessage(std::vector<uint8_t> msg, tcp::socket *socketClient, int id) {

    printf("VOUI\n");

    Deserializer d;
    Request_CTS rqFC;

    d.setData(msg);
    d.deserialize(rqFC);
    d.clear();

    printf("RECU = %d : %d-%d\n", rqFC.tetroMsg.tetro.getType(), rqFC.tetroMsg.tetro.getX(), rqFC.tetroMsg.tetro.getY());

    switch(rqFC.type) {
        case Request_CTS::TYPE_TETROMINO_PLACED : 
                sendNewTetro(socketClient, id);
            break;
        case Request_CTS::TYPE_GAME_OVER : 

            break;        
        case Request_CTS::TYPE_CLIENT_CONNECTION_LOST : 

            break;
    }


    printf("BOB\n");


}

void sendGameStart(tcp::socket *socketClient, int id) {
        Serializer s;
        std::vector<uint8_t> serializedRequest;

        Tetromino firstTetro;
        Tetromino secondTetro;

        firstTetro.setRotation(0);
        firstTetro.setType(0);
        firstTetro.setPos({6,1});
        firstTetro.setType(rand()%7+1);

        secondTetro.setRotation(0);
        secondTetro.setType(0);
        secondTetro.setPos({6,1});
        secondTetro.setType(rand()%7+1);

        printf("%d type : %d\n", id, firstTetro.getType());
        printf("%d type : %d\n", id, secondTetro.getType());


        Request_STC rqSTC;
        rqSTC.type = Request_STC::TYPE_GAME_START;

        rqSTC.gameStart.firstTetro = firstTetro;
        rqSTC.gameStart.secondTetro = secondTetro;

        s.serialize(rqSTC);
        serializedRequest = s.getData();
        boost::asio::write(*socketClient, boost::asio::buffer(serializedRequest));
        s.clear();

}




int main(int argc, char* argv[]){


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

        ////////////////    GAME START  //////////////////////

        sendGameStart(sock1, 1);
        sendGameStart(sock2, 2);
        

        for(;;) {
            printf("???\n");

            if (queueCli1.poll(msg)) {
                printf("client 1 reçu\n"); 
                exploitMessage(msg, sock1, 1);
            }

            if (queueCli2.poll(msg)) {
                printf("client 2 reçu\n"); 
                exploitMessage(msg, sock1, 2);

            }

        }

    } catch (std::exception& e) {
        std::cerr << "Exception SERVER: " << e.what() << "\n";
    }
    


	return 0;

}