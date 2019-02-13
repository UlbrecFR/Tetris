#include "TetrisServer.h"

using boost::asio::ip::tcp;

Grid grids[NB_PLAYERS];
uint32_t scores[NB_PLAYERS];

static void clientListener(tcp::socket & socketClient, gf::Queue<std::vector<uint8_t>> & queueClient) {

    for(;;) {

        //on lit le message envoyé par le client
        std::vector<uint8_t> length(sizeof(uint64_t));

        boost::system::error_code error;
        socketClient.read_some(boost::asio::buffer(length), error);

        Deserializer ds(length);

        uint64_t size;
        ds.deserialize(size);

        std::vector<uint8_t> msg(size);

        socketClient.read_some(boost::asio::buffer(msg), error);

        //on gère les erreurs
        if (error == boost::asio::error::eof)
            break; // Connection closed cleanly by peer.
        else if (error)
            throw boost::system::system_error(error); // Some other error.

        queueClient.push(msg);
    } 
}

void sendNewTetro(std::vector<tcp::socket> & socketClients, size_t id) {
    Serializer s;
    Tetromino t;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(1, 7);

    t.setRotation(0);
    t.setPos({6,1});
    t.setType(dist(gen));

    Request_STC rqSTC;
    rqSTC.type = Request_STC::TYPE_NEW_TETROMINO;
    rqSTC.newTetroMsg.newTetro = t;

    s.serialize(rqSTC);
    boost::asio::write(socketClients[id], boost::asio::buffer(s.getData()));
    printf("Sending a TYPE_NEW_TETROMINO msg to Client %zu\n \ttetro : t%d-r%d\n", id, rqSTC.newTetroMsg.newTetro.getType(), rqSTC.newTetroMsg.newTetro.getRotation());
    s.clear();
}

void sendGrids(std::vector<tcp::socket> & socketClients, size_t id) {
    Serializer s;

    Request_STC rqSTC;

    for (size_t i = 0; i < NB_PLAYERS; ++i){
        if (i == id){
            rqSTC.type = Request_STC::TYPE_UPDATE;
            rqSTC.updateMsg.grid = grids[id];
            rqSTC.updateMsg.score = scores[id];
            printf("Sending a TYPE_UPDATE msg \n");
        } else {
            rqSTC.type = Request_STC::TYPE_UPDATE_OTHER;
            rqSTC.updateOtherMsg.grid = grids[id];
            rqSTC.updateOtherMsg.score = scores[id];
            printf("Sending a TYPE_UPDATE_OTHER msg \n");
        }

        s.serialize(rqSTC);
        boost::asio::write(socketClients[i], boost::asio::buffer(s.getData()));
        s.clear();
    }
}

void sendGameOver(std::vector<tcp::socket> & socketClients) {
    Serializer s;

    Request_STC rqSTC;
    rqSTC.type = Request_STC::TYPE_GAME_OVER;

    size_t winnerScore = 0;
    size_t nbWinner = 0;

    for (size_t i = 0; i < NB_PLAYERS; ++i){
        if (scores[i] > winnerScore){
            winnerScore = scores[i];
            nbWinner = 1;
        } else if (scores[i] == winnerScore){
            ++nbWinner;
        }
    }

     for (size_t i = 0; i < NB_PLAYERS; ++i){
        if (scores[i] == winnerScore){
            if (nbWinner > 1){
               rqSTC.gameOver.results = STC_GameOver::TYPE_DRAW;
               printf("Sending a draw message at player %zu\n", i); 
            } else if (nbWinner == 1){
               rqSTC.gameOver.results = STC_GameOver::TYPE_WIN;
               printf("Sending a win message at player %zu\n", i); 
            }    
        } else {
            rqSTC.gameOver.results = STC_GameOver::TYPE_LOOSE;           
            printf("Sending a loose message at player %zu\n", i);
        }
        s.serialize(rqSTC);
        boost::asio::write(socketClients[i], boost::asio::buffer(s.getData()));
        s.clear();
    }  
}

void sendBonus(size_t nbLine, size_t id, std::vector<tcp::socket> & socketClients){
    Serializer s;

    Request_STC rqSTC;
    rqSTC.type = Request_STC::TYPE_BONUS;

    rqSTC.bonus.typeBonus = nbLine;

    for (size_t i = 0; i < NB_PLAYERS; ++i){
        if (i == id){
            rqSTC.bonus.target = 0;
            printf("Sending a TYPE_BONUS msg \n");
        } else {
            rqSTC.bonus.target = 1;
            printf("Sending a TYPE_BONUS_OTHER msg \n");
        }

        s.serialize(rqSTC);
        boost::asio::write(socketClients[i], boost::asio::buffer(s.getData()));
        s.clear();

    }
}

void updateGrid(Tetromino t, size_t id, std::vector<tcp::socket> & socketClients){
    grids[id].addTetromino(t);
    if (grids[id].gameOver()){
        grids[id].clear();
        scores[id] /= 2;
    } else {
        size_t nbLine = grids[id].deleteLines();
        if(nbLine > 0){
            scores[id] += nbLine * nbLine;
            //if (nbLine > 1) {
                sendBonus(nbLine, id, socketClients);
            //}
        }  
    }
}



void exploitMessage(std::vector<uint8_t> & msg, std::vector<tcp::socket> & socketClients, size_t id) {

    Deserializer d;
    Request_CTS rqFC;

    d.setData(msg);
    d.deserialize(rqFC);
    d.clear();

    switch(rqFC.type) {
        case Request_CTS::TYPE_TETROMINO_PLACED : 
            printf("Received a TYPE_TETROMINO_PLACED msg from Client %zu\n\t placed-tetro : t%d r%d pos%d-%d\n", id, rqFC.tetroMsg.tetro.getType(), rqFC.tetroMsg.tetro.getRotation(), rqFC.tetroMsg.tetro.getX(), rqFC.tetroMsg.tetro.getY());
            updateGrid(rqFC.tetroMsg.tetro, id, socketClients);
            sendGrids(socketClients, id);
            sendNewTetro(socketClients, id);
            break;    
        case Request_CTS::TYPE_CLIENT_CONNECTION_LOST :
            printf("Received a TYPE_CLIENT_CONNECTION_LOST\n");   
            sendGameOver(socketClients);
            exit(0);
            break;
    }
}

void sendGameStart(tcp::socket & socketClient, uint64_t time, size_t id) {
        Serializer s;

        Tetromino firstTetro;
        Tetromino secondTetro;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dist(1, 7);

        firstTetro.setRotation(0);
        firstTetro.setPos({6,1});
        firstTetro.setType(dist(gen));

        secondTetro.setRotation(0);
        secondTetro.setPos({6,1});
        secondTetro.setType(dist(gen));

        printf("Sending a TYPE_GAME_START msg to Client %zu\n \ttetro : t%d-r%d\n\tnext-tetro : t%d-r%d\n", id, firstTetro.getType(), firstTetro.getRotation(), secondTetro.getType(), secondTetro.getRotation());

        Request_STC rqSTC;
        rqSTC.type = Request_STC::TYPE_GAME_START;

        rqSTC.gameStart.firstTetro = firstTetro;
        rqSTC.gameStart.secondTetro = secondTetro;
        rqSTC.gameStart.time = time;

        s.serialize(rqSTC);
        boost::asio::write(socketClient, boost::asio::buffer(s.getData()));
        s.clear();
}

int main(int argc, char* argv[]){
    try {
        if (argc != 3) {
            std::cerr << "Usage: ./TetrisServer <port> <gametime>\n";
            return 1;
        }

        gf::Clock clock;

        boost::asio::io_service ioService;
        tcp::acceptor a(ioService, tcp::endpoint(tcp::v4(), std::atoi(argv[1])));

        std::vector<tcp::socket> socketClients;
        gf::Queue<std::vector<uint8_t>> queueClients[NB_PLAYERS];

        for (size_t i = 0; i < NB_PLAYERS; ++i){
            tcp::socket sock(ioService);
            socketClients.push_back(std::move(sock));
        }
        for (size_t i = 0; i < NB_PLAYERS; ++i){a.accept(socketClients[i]);}
        for (size_t i = 0; i < NB_PLAYERS; ++i){std::thread(clientListener, std::ref(socketClients[i]), std::ref(queueClients[i])).detach();}

        std::vector<uint8_t> msg;

        ////////////////    GAME START  //////////////////////

        printf("Waiting for clients...\n");

        for (size_t i = 0; i < NB_PLAYERS; ++i){
            sendGameStart(socketClients[i], atoi(argv[2]), i);
        }

        gf::Time time;
        gf::Time gameDuration(gf::seconds(static_cast<float>(atoi(argv[2]))));

        for(;;) {
            for (size_t i = 0; i < NB_PLAYERS; ++i){
                if (queueClients[i].poll(msg)) {
                    exploitMessage(msg, socketClients, i);
                }
            }
            time = clock.getElapsedTime();
            if (time >= gameDuration){
                break;
            }
        }

        sendGameOver(socketClients);

    } catch (std::exception& e) {
        std::cerr << "Exception SERVER: " << e.what() << "\n";
    }
    return 0;

}