#include "TetrisServer.h"

using boost::asio::ip::tcp;

Grid grids[NB_PLAYERS];
uint32_t scores[NB_PLAYERS];

struct PlayerMalusTimer {
    uint8_t malusActive;
    gf::Time remainingTime;
    gf::Time lastTime;
    uint8_t target;
};

struct AntiCheat {
    gf::Time sendTime;
    uint8_t strikes;
};

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

        ds.reset(msg);
        Request_CTS rqFC;
        ds.deserialize(rqFC);

        queueClient.push(msg);

        if (rqFC.type == Request_CTS::TYPE_CLIENT_CONNECTION_LOST){
            break;
        }
    } 
}

void sendNewTetro(std::vector<tcp::socket> & socketClients, size_t id, std::vector<AntiCheat> & antiCheats, gf::Clock clk) {
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
    printf("Sending a TYPE_NEW_TETROMINO msg to Client %zu\n \ttetro : t%d-r%d\n", id, rqSTC.newTetroMsg.newTetro.getType(), 
        rqSTC.newTetroMsg.newTetro.getRotation());
    s.clear();
    antiCheats[id].sendTime = clk.getElapsedTime();
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

void sendConnectionLost(std::vector<tcp::socket> & socketClients) {
    Serializer s;

    Request_STC rqSTC;

    for (size_t i = 0; i < NB_PLAYERS; ++i){
        
            rqSTC.type = Request_STC::TYPE_CONNECTION_LOST;
            s.serialize(rqSTC);
            boost::asio::write(socketClients[i], boost::asio::buffer(s.getData()));
        
        s.clear();
    }  
}

void destroyLineMalus(size_t id) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(1, 7);
    
    Tetromino t;

    t.setRotation(1);    
    t.setType(dist(gen));
    
    std::uniform_int_distribution<size_t> distPos(1, WIDTH);
    t.setPos({static_cast<unsigned int>(distPos(gen)),HEIGHT-2});

    std::vector<gf::Vector2u> cases = t.getCases();

    for (gf::Vector2u c : cases) {
        grids[id](c.x, c.y) = 0;
    }

}

void sendMalusStart(size_t nbLine, size_t id, std::vector<tcp::socket> & socketClients, std::vector<PlayerMalusTimer> & timersMalus, 
                    gf::Clock & clk, gf::Queue<uint8_t> queueMalus[2]){
    if (timersMalus[timersMalus[id].target].malusActive != 0) {
        queueMalus[timersMalus[id].target].push(nbLine);
        return;
    } 

    Serializer s;

    Request_STC rqSTC;
    rqSTC.type = Request_STC::TYPE_MALUS_START;

    rqSTC.malusStart.typeMalus = nbLine;

    for (size_t i = 0; i < NB_PLAYERS; ++i){
        if (i == id){
            if (nbLine != 4) {
                rqSTC.malusStart.target = 0;
                printf("Sending a TYPE_MALUS_START msg to %zu \n", i);   
            }
        } else {
            if (nbLine == 4) {
                destroyLineMalus(i);
                sendGrids(socketClients, i);
                timersMalus[i].remainingTime.addTo(gf::seconds(2.0f));
            } else {
                timersMalus[i].remainingTime.addTo(gf::seconds(10.0f));
            }
                rqSTC.malusStart.target = 1;
                timersMalus[i].malusActive = true;
                timersMalus[i].lastTime = clk.getElapsedTime();
                printf("Sending a TYPE_MALUS_START msg to %zu \n", i);
        }

        s.serialize(rqSTC);
        boost::asio::write(socketClients[i], boost::asio::buffer(s.getData()));
        s.clear();

    }
}

void sendMalusEnd(size_t id, std::vector<tcp::socket> & socketClients){
    Serializer s;

    Request_STC rqSTC;
    rqSTC.type = Request_STC::TYPE_MALUS_END;

    for (size_t i = 0; i < NB_PLAYERS; ++i){
        if (i == id){
            rqSTC.malusEnd.target = 1;
            printf("Sending a TYPE_MALUS_END OTHER msg to %zu \n", i);
        } else {
            rqSTC.malusEnd.target = 0;
            printf("Sending a TYPE_MALUS_END SELF msg to %zu \n", i);        
        }

        s.serialize(rqSTC);
        boost::asio::write(socketClients[i], boost::asio::buffer(s.getData()));
        s.clear();

    }
}

void updateGrid(Tetromino t, size_t id, std::vector<tcp::socket> & socketClients, std::vector<PlayerMalusTimer> & timersMalus, 
                    gf::Clock & clk, gf::Queue<uint8_t> queuesMalus[2]){
    grids[id].addTetromino(t);
    if (grids[id].gameOver()){
        grids[id].clear();
        scores[id] /= 2;
    } else {
        size_t nbLine = grids[id].deleteLines();
        if(nbLine > 0){
            scores[id] += nbLine * nbLine;
            if (nbLine > 1) {
                sendMalusStart(nbLine, id, socketClients, timersMalus, clk, queuesMalus);
            }
        }  
    }
}



bool exploitMessage(std::vector<uint8_t> & msg, std::vector<tcp::socket> & socketClients, size_t id, std::vector<PlayerMalusTimer> & timersMalus, 
    gf::Queue<uint8_t> queuesMalus[2], gf::Clock & clk, std::vector<AntiCheat> & antiCheats) {

    Deserializer d;
    Request_CTS rqFC;

    d.setData(msg);
    d.deserialize(rqFC);
    d.clear();


    switch(rqFC.type) {
        case Request_CTS::TYPE_TETROMINO_PLACED : {
            printf("Received a TYPE_TETROMINO_PLACED msg from Client %zu\n\t placed-tetro : t%d r%d pos%d-%d\n", id, 
                rqFC.tetroMsg.tetro.getType(), rqFC.tetroMsg.tetro.getRotation(), rqFC.tetroMsg.tetro.getX(), rqFC.tetroMsg.tetro.getY());
            
            gf::Time timeReceive = clk.getElapsedTime();

            uint8_t bottom = 0;

            for (auto c : rqFC.tetroMsg.tetro.getCases()) {
                if (c.y > bottom) {
                    bottom = c.y;
                } 
            }

            if (timeReceive.asSeconds() - antiCheats[id].sendTime.asSeconds() > gf::seconds(1.0f).asSeconds() * bottom + gf::seconds(2.5f).asSeconds()) {
                ++antiCheats[id].strikes;
                if (antiCheats[id].strikes >= 3) {
                    scores[id] = 0;
                    sendGameOver(socketClients);
                    sendConnectionLost(socketClients);
                }
            }

            updateGrid(rqFC.tetroMsg.tetro, id, socketClients, timersMalus, clk, queuesMalus);
            sendGrids(socketClients, id);
            sendNewTetro(socketClients, id, antiCheats, clk);
            break;
        }
        case Request_CTS::TYPE_CLIENT_CONNECTION_LOST : {
            printf("Received a TYPE_CLIENT_CONNECTION_LOST\n");   
            sendConnectionLost(socketClients);
            return false;
            break;
        }
    }
    return true;
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

        printf("Sending a TYPE_GAME_START msg to Client %zu\n \ttetro : t%d-r%d\n\tnext-tetro : t%d-r%d\n", id, firstTetro.getType(), 
            firstTetro.getRotation(), secondTetro.getType(), secondTetro.getRotation());

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

        /////////////////////////// MALUS ///////////////////////
        PlayerMalusTimer timerMalus1;
        gf::Time timer1(gf::seconds(0.0f));
        timerMalus1.malusActive = 0;
        timerMalus1.remainingTime = std::move(timer1);
        timerMalus1.target = 1;

        PlayerMalusTimer timerMalus2;
        gf::Time timer2(gf::seconds(0.0f));
        timerMalus2.malusActive = 0;
        timerMalus2.remainingTime = std::move(timer2);
        timerMalus2.target = 0;

        std::vector<PlayerMalusTimer> timersMalus;
        timersMalus.push_back(timerMalus1);
        timersMalus.push_back(timerMalus2);

        gf::Queue<uint8_t> queuesMalus[NB_PLAYERS];

        /////////////////VAC//////////////////

        std::vector<AntiCheat> antiCheats;
        AntiCheat VACpl1;
        VACpl1.strikes = 0;
        VACpl1.sendTime = gf::Time(gf::seconds(0.0f));

        AntiCheat VACpl2;
        VACpl2.strikes = 0;
        VACpl2.sendTime = gf::Time(gf::seconds(0.0f));

        antiCheats.push_back(VACpl1);
        antiCheats.push_back(VACpl2);

        ////////////////    GAME START  //////////////////////

        printf("Waiting for clients...\n");

        for (size_t i = 0; i < NB_PLAYERS; ++i){
            sendGameStart(socketClients[i], atoi(argv[2]), i);
        }

        gf::Clock clock;

        gf::Time time;
        gf::Time gameDuration(gf::seconds(static_cast<float>(atoi(argv[2]))));
        gf::Time timeMalusI;

        for(;;) {
            for (size_t i = 0; i < NB_PLAYERS; ++i){
                if (queueClients[i].poll(msg)) {
                    if (!exploitMessage(msg, socketClients, i, timersMalus, queuesMalus, clock, antiCheats)){
                        return 0;
                    }
                }

                if (timersMalus[i].malusActive != 0) {  
                    timeMalusI = clock.getElapsedTime();
                    timersMalus[i].remainingTime.subTo(timeMalusI - timersMalus[i].lastTime);
                    timersMalus[i].lastTime = timeMalusI;

                    if (timersMalus[i].remainingTime <= gf::seconds(0.0f)) {
                        sendMalusEnd(i, socketClients);
                        timersMalus[i].malusActive = 0;
                        uint8_t nextMalus;
                        if (queuesMalus[i].poll(nextMalus)) {
                            sendMalusStart(nextMalus, timersMalus[i].target, socketClients, timersMalus, clock, queuesMalus);
                        }
                    }
                }
            }

            time = clock.getElapsedTime();
            if (time >= gameDuration){
                break;
            }
        }

        sendGameOver(socketClients);
        sendConnectionLost(socketClients);

    } catch (std::exception& e) {
        std::cerr << "Exception SERVER: " << e.what() << "\n";
    }
    return 0;

}