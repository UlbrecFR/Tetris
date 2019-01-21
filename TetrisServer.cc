#include "TetrisServer.h"

using boost::asio::ip::tcp;

static void clientListener(tcp::socket *socketClient, gf::Queue<std::vector<uint8_t>> *queueClient) {

    for(;;) {

        //on lit le message envoyé par le client
        std::vector<uint8_t> length(sizeof(uint64_t));

        boost::system::error_code error;
        socketClient->read_some(boost::asio::buffer(length), error);

        Deserializer ds(length);

        uint64_t size;
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

void sendNewTetro(tcp::socket *socketClient, int id) {
    Serializer s;
    Tetromino t;

    t.setRotation(0);
    t.setPos({6,1});
    t.setType(rand()%7+1);

    Request_STC rqSTC;
    rqSTC.type = Request_STC::TYPE_NEW_TETROMINO;
    rqSTC.newTetroMsg.newTetro = t;

    s.serialize(rqSTC);
    boost::asio::write(*socketClient, boost::asio::buffer(s.getData()));
    printf("Sending a TYPE_NEW_TETROMINO msg to Client %d\n \ttetro : t%d-r%d\n", id, rqSTC.newTetroMsg.newTetro.getType(), rqSTC.newTetroMsg.newTetro.getRotation());
    s.clear();
}

void sendGrid(tcp::socket *socketClient, Grid & g, int id) {
    Serializer s;

    Request_STC rqSTC;
    rqSTC.type = Request_STC::TYPE_UPDATE_OTHER;
    rqSTC.updateOtherMsg.grid = g;

    s.serialize(rqSTC);
    boost::asio::write(*socketClient, boost::asio::buffer(s.getData()));
    printf("Sending a TYPE_UPDATE_OTHER msg \n");
    s.clear();
}

void sendGameOver(tcp::socket *socketLooser, tcp::socket *socketWinner) {
    Serializer s;

    Request_STC rqSTC;
    rqSTC.type = Request_STC::TYPE_GAME_OVER;
    rqSTC.gameOver.win = true;

    s.serialize(rqSTC);
    boost::asio::write(*socketWinner, boost::asio::buffer(s.getData()));
    printf("Sending a win message \n");
    s.clear();

    rqSTC.gameOver.win = false;

    s.serialize(rqSTC);
    boost::asio::write(*socketLooser, boost::asio::buffer(s.getData()));
    printf("Sending a Loose message \n");
    s.clear();
}

void exploitMessage(std::vector<uint8_t> & msg, tcp::socket *socketClient, tcp::socket *socketOtherClient, int id) {

    Deserializer d;
    Request_CTS rqFC;

    d.setData(msg);
    d.deserialize(rqFC);
    d.clear();

    switch(rqFC.type) {
        case Request_CTS::TYPE_TETROMINO_PLACED : 
            printf("Received a TYPE_TETROMINO_PLACED msg from Client %d\n\t placed-tetro : t%d r%d pos%d-%d\n", id, rqFC.tetroMsg.tetro.getType(), rqFC.tetroMsg.tetro.getRotation(), rqFC.tetroMsg.tetro.getX(), rqFC.tetroMsg.tetro.getY());
            sendNewTetro(socketClient, id);
            sendGrid(socketOtherClient, rqFC.tetroMsg.grid, id);
            break;
        case Request_CTS::TYPE_GAME_OVER :
            printf("Received a TYPE_GAME_OVER\n");   
            sendGameOver(socketClient, socketOtherClient);
            break;        
        case Request_CTS::TYPE_CLIENT_CONNECTION_LOST : 
            break;
    }
}

void sendGameStart(tcp::socket *socketClient, int id) {
        Serializer s;

        Tetromino firstTetro;
        Tetromino secondTetro;

        firstTetro.setRotation(0);
        firstTetro.setPos({6,1});
        firstTetro.setType(rand()%7+1);

        secondTetro.setRotation(0);
        secondTetro.setPos({6,1});
        secondTetro.setType(rand()%7+1);

        printf("Sending a TYPE_GAME_START msg to Client %d\n \ttetro : t%d-r%d\n\tnext-tetro : t%d-r%d\n", id, firstTetro.getType(), firstTetro.getRotation(), secondTetro.getType(), secondTetro.getRotation());

        Request_STC rqSTC;
        rqSTC.type = Request_STC::TYPE_GAME_START;

        rqSTC.gameStart.firstTetro = firstTetro;
        rqSTC.gameStart.secondTetro = secondTetro;

        s.serialize(rqSTC);
        boost::asio::write(*socketClient, boost::asio::buffer(s.getData()));
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

        printf("Waiting for clients...\n");

        sendGameStart(sock1, 1);
        sendGameStart(sock2, 2);

        for(;;) {
            if (queueCli1.poll(msg)) {
                exploitMessage(msg, sock1, sock2, 1);
                printf("finExlpoit\n");
            }

            if (queueCli2.poll(msg)) {
                exploitMessage(msg, sock2, sock1, 2);
            }
        }

    } catch (std::exception& e) {
        std::cerr << "Exception SERVER: " << e.what() << "\n";
    }
    


    return 0;

}