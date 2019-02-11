#include "TetrisClient.h"

using boost::asio::ip::tcp;

static void serverListener(tcp::socket & socketServer, gf::Queue<std::vector<uint8_t>> & queueServer) {
    for(;;) {
        std::vector<uint8_t> length(sizeof(uint64_t));

        boost::system::error_code error;
        socketServer.read_some(boost::asio::buffer(length), error);

        Deserializer ds(length);
        uint64_t size;
        ds.deserialize(size);

        std::vector<uint8_t> msg(size);

        socketServer.read_some(boost::asio::buffer(msg), error);

        //on gère les erreurs
        if (error == boost::asio::error::eof)
            break; // Connection closed cleanly by peer.
        else if (error)
            throw boost::system::system_error(error); // Some other error.

        queueServer.push(msg);
    }
}

static void updateScreenSize(gf::Window & window, gf::LockedView & mainView){
    for (;;){
        if(window.getSize().x < 24*SIZE_CASE){
            window.setSize({24*SIZE_CASE, window.getSize().y});
        }
        if(window.getSize().y < 19*SIZE_CASE){
            window.setSize({window.getSize().x, 19*SIZE_CASE});
        }
        mainView.onScreenSizeChange(window.getSize());
    }  
}

void sendClientDisco(bool crash, tcp::socket & sock){
    Serializer s;
    Request_CTS rqTS;
    std::vector<uint8_t> request;

    rqTS.type = Request_CTS::TYPE_CLIENT_CONNECTION_LOST;
    rqTS.discoMsg.error = CTS_ClientConnectionLost::TYPE_CLIENT_QUIT;
    if (crash){rqTS.discoMsg.error = CTS_ClientConnectionLost::TYPE_CLIENT_CRASH;}

    s.serialize(rqTS);

    request = s.getData();
    boost::asio::write(sock, boost::asio::buffer(request, request.capacity()));

    printf("Sending a TYPE_CLIENT_CONNECTION_LOST msg\n");

    s.clear();
}

int main(int argc, char* argv[]){

    printf("%s\n", "Tetris Client : Le Vouitris");

    if (argc != 3) {
        std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
        return 1;
    }

    try {

        ///////////////////////////////////////////

        // initialization

        static constexpr gf::Vector2f ViewSize(24*SIZE_CASE, 19*SIZE_CASE); // dummy values
        static constexpr gf::Vector2f ViewCenter(24*SIZE_CASE/2, 19*SIZE_CASE/2); // dummy values

        gf::Window window("Vouitris", ViewSize);
        window.setVerticalSyncEnabled(true);
        window.setFramerateLimit(60);
        gf::RenderWindow renderer(window);
        gf::RenderStates r_state;

        DisplayGame displayGame;

        /////////////////////////////////////////////////////////////

        // views
        gf::ViewContainer views;
        gf::LockedView mainView(ViewCenter, ViewSize);
        views.addView(mainView);
        gf::ScreenView hudView;
        views.addView(hudView);
        views.setInitialScreenSize(ViewSize);

        std::thread(updateScreenSize, std::ref(window), std::ref(mainView)).detach();

        // actions
        Controls controls;  
        uint8_t malus = 0;
        bool malus_other = false;

        //////////////////////////////////////////////////////////////

        while (window.isOpen()){

            renderer.clear(gf::Color::fromRgba32(50,50,50,255));
            renderer.setView(mainView);
            displayGame.drawWaitServer(renderer);
            renderer.display();

            try {
                gf::Event event;

                while (window.pollEvent(event)) {
                    controls.processEvent(event);
                }

                if (controls("Close").isActive()) {
                    window.close();
                    return 0;
                }  

                boost::asio::io_service io_service;

                tcp::socket sock(io_service);
                tcp::resolver resolver(io_service);
                boost::asio::connect(sock, resolver.resolve({argv[1], argv[2]}));

                gf::Queue<std::vector<uint8_t>> queueServer;

                std::thread(serverListener, std::ref(sock), std::ref(queueServer)).detach();

                try {
            
                    bool enPartie = true;
                    uint8_t win = 0;     
                    uint32_t scoreSelf = 0;
                    uint32_t scoreOther = 0;
                    Tetromino currentTetro;
                    Tetromino nextTetro;
                    std::vector<uint8_t> msg;
                    Grid gdSelf; 
                    Grid gdOther; 
                    gdSelf.clear();
                    gdOther.clear();

                    // game loop
                    gf::Clock clockChute;
                    gf::Time t;
                    gf::Time periodChute = gf::seconds(1.0f);

                //////////////////////////////////////////////////////////
                    
                    Deserializer d;
                    Serializer s;
                    std::vector<uint8_t> request;

                    Request_STC rqFS;
                    Request_CTS rqTS;

                    printf("Connecting to server...\n");

                    while (!queueServer.poll(msg)) {
                        while (window.pollEvent(event)) {
                            controls.processEvent(event);
                        }

                        if (controls("Close").isActive()) {
                            sendClientDisco(false, sock);
                            window.close();
                            return 0;
                        } 

                        renderer.clear(gf::Color::fromRgba32(50,50,50,255));
                        renderer.setView(mainView);    
                        displayGame.drawWaitPlayer(renderer);
                        renderer.display();
                    }

                    printf("Connected\n");

                    d.setData(msg);
                    d.deserialize(rqFS);
                    d.clear();
                    
                    if (rqFS.type == Request_STC::TYPE_GAME_START) {
                        currentTetro = rqFS.gameStart.firstTetro;
                        nextTetro=rqFS.gameStart.secondTetro;
                        printf("Received a TYPE_GAME_START msg\n \ttetro : t%d-r%d\n\tnext-tetro : t%d-r%d\n", currentTetro.getType(), currentTetro.getRotation(), nextTetro.getType(), nextTetro.getRotation());
                    }

                    gf::Clock clock;
                    gf::Time pastTime;
                    gf::Time gameDuratiion = (gf::seconds(static_cast<float>(rqFS.gameStart.time)));
                    gf::Time time;

                    while (enPartie) {

                        // 1. input

                        while (window.pollEvent(event)) {
                            controls.processEvent(event);
                        }

                        if (controls("Close").isActive()) {
                            sendClientDisco(false, sock);
                            window.close();
                            return 0;
                        }                

                        if (queueServer.poll(msg)) { //RECEPTION SERVER
                            d.setData(msg);
                            d.deserialize(rqFS);
                            d.clear();
                            
                            switch (rqFS.type) {
                                case Request_STC::TYPE_NEW_TETROMINO :
                                    nextTetro.setPos(rqFS.newTetroMsg.newTetro.getPos());
                                    nextTetro.setRotation(rqFS.newTetroMsg.newTetro.getRotation());
                                    nextTetro.setType(rqFS.newTetroMsg.newTetro.getType());
                                    printf("Received a TYPE_NEW_TETROMINO msg\n\tnext-tetro : t%d-r%d\n", nextTetro.getType(), nextTetro.getRotation());
                                    break;
                                case Request_STC::TYPE_UPDATE :
                                    gdSelf = rqFS.updateMsg.grid;
                                    scoreSelf = rqFS.updateMsg.score;
                                    printf("Received a TYPE_UPDATE\n");
                                    break;
                                case Request_STC::TYPE_UPDATE_OTHER :
                                    gdOther = rqFS.updateOtherMsg.grid;
                                    scoreOther = rqFS.updateOtherMsg.score;
                                    malus_other = false;
                                    printf("Received a TYPE_UPDATE_OTHER\n");
                                    break;
                                case Request_STC::TYPE_GAME_START :
                                    printf("Received a TYPE_GAME_START\n");
                                    break;
                                case Request_STC::TYPE_GAME_OVER :
                                    printf("Received a TYPE_GAME_OVER\n");
                                    enPartie = false;
                                    win = rqFS.gameOver.results;
                                    break;
                                case Request_STC::TYPE_BONUS :
                                    printf("%d\n", rqFS.bonus.target);
                                    printf("%d\n", rqFS.bonus.typeBonus);
                
                                    if(rqFS.bonus.target == 0){
                                        malus_other = true;
                                    }else{
                                        malus = rqFS.bonus.typeBonus;
                                    }
                                    break;
                            }
                        }
                        if (controls("Right").isActive() && malus != 3) {
                            if(gdSelf.rightPossible(currentTetro)){
                                gdSelf(currentTetro.getX(), currentTetro.getY()) = 0;
                                currentTetro.setX(currentTetro.getX() + 1);
                                gdSelf(currentTetro.getX(), currentTetro.getY()) = currentTetro.getType();
                            }
                            
                        } else if (controls("Left").isActive() && malus != 3) {
                            if (gdSelf.leftPossible(currentTetro)){
                                gdSelf(currentTetro.getX(), currentTetro.getY()) = 0;
                                currentTetro.setX(currentTetro.getX() - 1);
                                gdSelf(currentTetro.getX(), currentTetro.getY()) = currentTetro.getType();
                            }
                            
                        } else if (controls("Rotate").isActive() && malus != 2) {
                            if (gdSelf.rotatePossible(currentTetro)) {
                                currentTetro.rotate();
                            }
                        } 

                        if (controls("Down").isActive() && malus != 3) {
                            periodChute.subTo(gf::seconds(0.1f));
                        } else {                                
                            if (malus == 1) {
                                periodChute = gf::seconds(0.1f);
                            } else {
                                periodChute = gf::seconds(1.0f);
                            }
                        }

                        t = clockChute.getElapsedTime();

                       if(gdSelf.downPossible(currentTetro)){
                            if(t > periodChute){
                                gdSelf(currentTetro.getX(), currentTetro.getY()) = 0;
                                t = clockChute.restart();
                                currentTetro.setY(currentTetro.getY() + 1);
                                gdSelf(currentTetro.getX(), currentTetro.getY()) = currentTetro.getType();
                            }
                        }else{
                            if(t > periodChute){
                                malus = 0;
                                t = clockChute.restart();
                                gdSelf.addTetromino(currentTetro);
                                if (malus == 1) {
                                    periodChute = gf::seconds(0.1f);
                                } else {
                                    periodChute = gf::seconds(1.0f);
                                }
                                
                                rqTS.type = Request_CTS::TYPE_TETROMINO_PLACED;
                                rqTS.tetroMsg.tetro = currentTetro;

                                currentTetro = nextTetro;
                                
                                t = clockChute.restart();

                                s.serialize(rqTS);

                                printf("Sending a TYPE_TETROMINO_PLACED msg\n\t placed-tetro : t%d r%d pos%d-%d\n", rqTS.tetroMsg.tetro.getType(), rqTS.tetroMsg.tetro.getRotation(), rqTS.tetroMsg.tetro.getX(), rqTS.tetroMsg.tetro.getY());

                                request = s.getData();
                                boost::asio::write(sock, boost::asio::buffer(request, request.capacity()));
                                s.clear();
                            }
                        }

                        pastTime = clock.getElapsedTime();
                        time = gameDuratiion-pastTime; 

                        renderer.clear();
                        renderer.setView(mainView);       

                        displayGame.draw(gdSelf, gdOther, currentTetro, nextTetro, scoreSelf, scoreOther, time, renderer, r_state);
                 
                        renderer.display();
                        controls.reset();
                    }

                    time = gf::seconds(0.0f);

                    while(!enPartie){

                        while (window.pollEvent(event)) {
                            controls.processEvent(event);
                        }

                        if (controls("Close").isActive()) {
                            sendClientDisco(false, sock);
                            window.close();
                            return 0;
                        } 

                        renderer.clear();
                        renderer.setView(mainView);

                        displayGame.draw(gdSelf, gdOther, currentTetro, nextTetro, scoreSelf, scoreOther, time, renderer, r_state);

                        switch(win){
                            case STC_GameOver::TYPE_WIN:
                                displayGame.drawWin(renderer);
                                break;
                            case STC_GameOver::TYPE_LOOSE:
                                displayGame.drawLoose(renderer);
                                break;
                            case STC_GameOver::TYPE_DRAW:
                                displayGame.drawDraw(renderer);
                                break;  
                        }

                        renderer.display();
                        controls.reset();
                    }
                } catch (std::exception& e){
                    sendClientDisco(true, sock);
                    std::cerr << "Exception CLIENT : " << e.what() << "\n";
                    return 1;           
                }                  
                
            } catch (std::exception& e) {
                if (e.what() == std::string("connect: Connection refused")){
                    std::cerr << "Connection to server failed : will try again in "<< RETRY_CONNECTION_DELAY << " seconds\n";
                } else {
                    std::cerr << "Exception CLIENT : " << e.what() << "\n";
                    return 1;           
                }
            }

            //sleep(RETRY_CONNECTION_DELAY);
        }
    } catch (std::exception& e) {
        std::cerr << "Exception CLIENT : " << e.what() << "\n";
        return 1;           
    }

    
    return 0;
}