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


int main(int argc, char* argv[]){

    printf("%s\n", "Tetris Client : Le Vouitris");

    try {
        if (argc != 3) {
            std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        tcp::socket sock(io_service);
        tcp::resolver resolver(io_service);
        boost::asio::connect(sock, resolver.resolve({argv[1], argv[2]}));

        gf::Queue<std::vector<uint8_t>> queueServer;

        std::thread(serverListener, std::ref(sock), std::ref(queueServer)).detach();
    
///////////////////////////////////////////

    // initialization
        static constexpr uint8_t width = 12;
        static constexpr uint8_t height = 17;
        static constexpr int sizeCase = 40;

        static constexpr gf::Vector2u ScreenSize(30*sizeCase, 19*sizeCase);

        static constexpr gf::Vector2f ViewSize1(30*sizeCase, 17*sizeCase); // dummy values
        static constexpr gf::Vector2f ViewCenter1(14*sizeCase, 17*sizeCase/2); // dummy values7

        static constexpr gf::Vector2f ViewSize2(30*sizeCase, 17*sizeCase); // dummy values
        static constexpr gf::Vector2f ViewCenter2(-2*sizeCase, 17*sizeCase/2); // dummy values



        gf::Window window("Vouitris", ScreenSize);
        window.setVerticalSyncEnabled(true);
        window.setFramerateLimit(60);
        gf::RenderWindow renderer(window);


        Grid gaSelf;
        Grid gaOther;

        uint32_t score = 0;

       // GameArea ga(width, height);   
        ///////////////////////////////////////////////////////////////

        gf::Texture textureFond;
        if (!textureFond.loadFromFile(gf::Path("../ressources/fond.png"))) {
            return EXIT_FAILURE;
        }

        gf::Texture tabTexturePiece[7];

        gf::Texture texturePiece0;
        if (!texturePiece0.loadFromFile(gf::Path("../ressources/0.png"))) {
            return EXIT_FAILURE;
        }
        tabTexturePiece[0] = std::move(texturePiece0);

        gf::Texture texturePiece1;
        if (!texturePiece1.loadFromFile(gf::Path("../ressources/1.png"))) {
            return EXIT_FAILURE;
        }
        tabTexturePiece[1] = std::move(texturePiece1);

        gf::Texture texturePiece2;
        if (!texturePiece2.loadFromFile(gf::Path("../ressources/2.png"))) {
            return EXIT_FAILURE;
        }
        tabTexturePiece[2] = std::move(texturePiece2);

        gf::Texture texturePiece3;
        if (!texturePiece3.loadFromFile(gf::Path("../ressources/3.png"))) {
            return EXIT_FAILURE;
        }
        tabTexturePiece[3] = std::move(texturePiece3);

        gf::Texture texturePiece4;
        if (!texturePiece4.loadFromFile(gf::Path("../ressources/4.png"))) {
            return EXIT_FAILURE;
        }
        tabTexturePiece[4] = std::move(texturePiece4);

        gf::Texture texturePiece5;
        if (!texturePiece5.loadFromFile(gf::Path("../ressources/5.png"))) {
            return EXIT_FAILURE;
        }

        tabTexturePiece[5] = std::move(texturePiece5);

        gf::Texture texturePiece6;
        if (!texturePiece6.loadFromFile(gf::Path("../ressources/6.png"))) {
            return EXIT_FAILURE;
        }

        tabTexturePiece[6] = std::move(texturePiece6);
        
        gf::Sprite background;
        background.setTexture(textureFond);

        gf::Texture textureWin;
        if (!textureWin.loadFromFile(gf::Path("../ressources/win.png"))) {
            return EXIT_FAILURE;
        }

        gf::Texture textureLost;
        if (!textureLost.loadFromFile(gf::Path("../ressources/lost.png"))) {
            return EXIT_FAILURE;
        }

        gf::Sprite spriteGameOver;


        gf::Texture textureWait;
        if (!textureWait.loadFromFile(gf::Path("../ressources/waitingScreen.png"))) {
            return EXIT_FAILURE;
        }

        gf::Sprite spriteWait;
        spriteWait.setTexture(textureWait);



        //tableau zone de jeu de sprite
        gf::Sprite tabSprite[width][height];
        gf::Sprite tabSpriteOther[width][height];

        for (int i = 0; i < height; ++i){
            for (int j = 0; j < width; ++j){
                gf::Sprite sprite;
                gf::Sprite spriteOther;
                sprite.setPosition({j* sizeCase-2, i* sizeCase-2});
                spriteOther.setPosition({j* sizeCase-2, i* sizeCase-2});
                tabSprite[j][i] = std::move(sprite);
                tabSpriteOther[j][i] = std::move(spriteOther);
            }
            
        }

        /////////////////////////////////////////////////////////////

        /*
        Zone jeu : extend
        autour : screen
        */
            
       
        bool pieceEnJeu = false;
        bool enPartie = true;

        // views
        gf::ViewContainer views;
        gf::LockedView mainView(ViewCenter1, ViewSize1);
        gf::LockedView otherView(ViewCenter2, ViewSize2);
        views.addView(mainView);
        views.addView(otherView);
        gf::ScreenView hudView;
        views.addView(hudView);
        views.setInitialScreenSize(ScreenSize);

        // actions
        gf::ActionContainer actions;

        gf::Action closeWindowAction("Close window");
        closeWindowAction.addCloseControl();
        closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
        actions.addAction(closeWindowAction);


        gf::Action leftAction("Left");
        leftAction.addScancodeKeyControl(gf::Scancode::Q);
        leftAction.addScancodeKeyControl(gf::Scancode::Left);
        leftAction.setInstantaneous();
        actions.addAction(leftAction);

        gf::Action rightAction("Right");
        rightAction.addScancodeKeyControl(gf::Scancode::D);
        rightAction.addScancodeKeyControl(gf::Scancode::Right);
        rightAction.setInstantaneous();
        actions.addAction(rightAction);

        gf::Action rotateAction("rotate");
        rotateAction.addScancodeKeyControl(gf::Scancode::Space);
        rotateAction.setInstantaneous();
        actions.addAction(rotateAction);

        gf::Action downAction("Down");
        downAction.addScancodeKeyControl(gf::Scancode::S);
        downAction.addScancodeKeyControl(gf::Scancode::Down);
        downAction.setContinuous();
        actions.addAction(downAction);



    ///////////////////////////////////////////////////////////////

        Tetromino tetro;
        Tetromino next_tetro;
        std::vector<uint8_t> msg;
    

        // entities
        gf::EntityContainer mainEntities;

        // add entities to mainEntities
        gf::EntityContainer hudEntities;

        // add entities to hudEntities

        // game loop
        renderer.clear(gf::Color::fromRgba32(50,50,50,255));
        gf::Clock clock;
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


        renderer.draw(spriteWait);
        renderer.setView(hudView);
        hudEntities.render(renderer);

        renderer.display();

        while (!queueServer.poll(msg)) {
            //printf("wait for first tetro\n");
        }

        printf("Connected\n");

        d.setData(msg);
        d.deserialize(rqFS);
        d.clear();
        
        if (rqFS.type == Request_STC::TYPE_GAME_START) {
            tetro = rqFS.gameStart.firstTetro;
            next_tetro=rqFS.gameStart.secondTetro;
            printf("Received a TYPE_GAME_START msg\n \ttetro : t%d-r%d\n\tnext-tetro : t%d-r%d\n", tetro.getType(), tetro.getRotation(), next_tetro.getType(), next_tetro.getRotation());
        }

        pieceEnJeu = true;

        while (window.isOpen()) {

            if (queueServer.poll(msg)) { //RECEPTION SERVER
                d.setData(msg);
                d.deserialize(rqFS);
                d.clear();
                
                switch (rqFS.type) {

                    case Request_STC::TYPE_NEW_TETROMINO :
                        next_tetro.setPos(rqFS.newTetroMsg.newTetro.getPos());
                        next_tetro.setRotation(rqFS.newTetroMsg.newTetro.getRotation());
                        next_tetro.setType(rqFS.newTetroMsg.newTetro.getType());
                        printf("Received a TYPE_NEW_TETROMINO msg\n\tnext-tetro : t%d-r%d\n", next_tetro.getType(), next_tetro.getRotation());
                        break;
                    case Request_STC::TYPE_UPDATE_OTHER :
                        gaOther = rqFS.updateOtherMsg.grid;
                        printf("Received a TYPE_UPDATE_OTHER\n");
                        break;
                    case Request_STC::TYPE_GAME_START :
                        printf("Received a TYPE_UPDATE_OTHER\n");
                        break;
                    case Request_STC::TYPE_GAME_OVER :
                        printf("Received a TYPE_GAME_OVER\n");
                        enPartie = false;
                        if (rqFS.gameOver.win == false) {
                            spriteGameOver.setTexture(textureLost);
                        } else {
                            spriteGameOver.setTexture(textureWin);
                        }
                        break;
                }
            }

            // 1. input
            gf::Event event;

            while (window.pollEvent(event)) {
                actions.processEvent(event);
                //views.processEvent(event);
            }

            if (closeWindowAction.isActive()) {
                window.close();
            }

            if(enPartie){
                if(!pieceEnJeu){ //ENVOI SERVER

                    rqTS.type = Request_CTS::TYPE_TETROMINO_PLACED;
                    rqTS.tetroMsg.tetro = tetro;
                    rqTS.tetroMsg.grid = gaSelf;

                    tetro = next_tetro;
                    
                    pieceEnJeu = true;
                    gaSelf(tetro.getX(), tetro.getY()) = tetro.getType();
                    t = clockChute.restart();

                    s.serialize(rqTS);

                    printf("Sending a TYPE_TETROMINO_PLACED msg\n\t placed-tetro : t%d r%d pos%d-%d\n", rqTS.tetroMsg.tetro.getType(), rqTS.tetroMsg.tetro.getRotation(), rqTS.tetroMsg.tetro.getX(), rqTS.tetroMsg.tetro.getY());

                    request = s.getData();
                    boost::asio::write(sock, boost::asio::buffer(request, request.capacity()));
                    s.clear();
                }

                //printf("%f\n", t.asSeconds());

                if (rightAction.isActive()) {
                    if(gaSelf.rightPossible(tetro)){
                        gaSelf(tetro.getX(), tetro.getY()) = 0;
                        tetro.setX(tetro.getX() + 1);
                        gaSelf(tetro.getX(), tetro.getY()) = tetro.getType();
                        //printZoneJeu(tabJeu, height, width);
                    }
                    
                } else if (leftAction.isActive()) {
                    if (gaSelf.leftPossible(tetro)){
                        gaSelf(tetro.getX(), tetro.getY()) = 0;
                        tetro.setX(tetro.getX() - 1);
                        gaSelf(tetro.getX(), tetro.getY()) = tetro.getType();
                        //printZoneJeu(tabJeu, height, width);
                    }
                    
                } else if (rotateAction.isActive()) {
                    if (gaSelf.rotatePossible(tetro)) {
                        tetro.rotate();
                    }
                } 

                if (downAction.isActive()) {
                    periodChute.subTo(gf::seconds(0.1f));
                } else {
                    periodChute = gf::seconds(1.0f);
                }

                t = clockChute.getElapsedTime();

               if(gaSelf.downPossible(tetro)){
                    if(t > periodChute){
                        gaSelf(tetro.getX(), tetro.getY()) = 0;
                        t = clockChute.restart();
                        //printf("%f\n", t.asSeconds());
                        //printf("Chute\n");
                        //printf("%f\n", periodChute.asSeconds());
                        tetro.setY(tetro.getY() + 1);
                        gaSelf(tetro.getX(), tetro.getY()) = tetro.getType();
                        //printZoneJeu(gaSelf);
                    }
                }else{
                    pieceEnJeu = false;
                    gaSelf.addTetromino(tetro);
                    periodChute = gf::seconds(1.0f);
                    size_t nbLine = gaSelf.deleteLines();
                    if(nbLine > 0){
                        score += nbLine * nbLine;
                        printf("Score : %d\n", score);
                    }
                }


                for (uint8_t i = 0; i < height; ++i){
                    for (uint8_t j = 0; j < width; ++j){
                        if(gaSelf(j, i) > 0){
                            tabSprite[j][i].setTexture(tabTexturePiece[(gaSelf(j,i))-1], true);  
                        } else {
                            tabSprite[j][i].unsetTexture();
                        }

                        if(gaOther(j, i) > 0){
                            tabSpriteOther[j][i].setTexture(tabTexturePiece[(gaOther(j,i))-1], true);  
                        } else {
                            tabSpriteOther[j][i].unsetTexture();
                        }

                    }
                }

                auto listeCurrentCase = tetro.getCases();

                for (auto it : listeCurrentCase){
                    if(it.y>=0){
                        tabSprite[it.x][it.y].setTexture(tabTexturePiece[tetro.getType()-1], true);
                    }                
                }

                if (!gaSelf.gameOver(tetro)) {
                    enPartie = false;

                    rqTS.type = Request_CTS::TYPE_GAME_OVER;
                    rqTS.tetroMsg.tetro = tetro;
                    
                    s.serialize(rqTS);

                    printf("Sending Game Over message\n");

                    request = s.getData();
                    boost::asio::write(sock, boost::asio::buffer(request, request.capacity()));
                    s.clear();
                }
            }


            // 2. update
            gf::Time time = clock.restart();
            mainEntities.update(time);
            hudEntities.update(time);

            // 3. draw
            renderer.clear();
            renderer.setView(mainView);
            mainEntities.render(renderer);

            renderer.draw(background);
            
            for (int i = 0; i < height; ++i){
                for (int j = 0; j < width; ++j){
                    renderer.draw(tabSprite[j][i]);
                }  
            }

            renderer.setView(otherView);

            renderer.draw(background);

            for (int i = 0; i < height; ++i){
                for (int j = 0; j < width; ++j){
                    renderer.draw(tabSpriteOther[j][i]);
                }  
            }

            if (!enPartie) {
                renderer.setView(mainView);
                renderer.draw(spriteGameOver);
            }

            renderer.setView(hudView);
            hudEntities.render(renderer);

            renderer.display();
            actions.reset();

        }


    } catch (std::exception& e) {
        std::cerr << "Exception CLIENT : " << e.what() << "\n";
    }

    return 0;
}