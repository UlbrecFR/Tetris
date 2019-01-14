#include "TetrisClient.h"

using boost::asio::ip::tcp;


void printZoneJeu(int tab[12][17], int height, int width){
    for (int i = 0; i < height; ++i){
        for(int j = 0; j < width; ++j){
            printf("%d ", tab[j][i]);
        }
        printf("\n");
    }
    printf("\n");
}

void printEtatJeu(int tab[12][17], int height, int width, Tetromino tetro){
    std::set<std::pair<int, int>> listeCase = tetro.getCases();
    for (std::set<std::pair<int, int>>::const_iterator it = listeCase.begin(); it != listeCase.end(); it++){
        if(it->second>0){
            tab[it->first][it->second] = tetro.getType();
        }
        
    }

}

void chuteLigneSuppr(int tab[12][17], int height, int width, int nLigne){
    for (int i = nLigne; i>=0; --i){
        for(int j = 0; j < width; ++j){
            if(tab[j][i]>0){
                tab[j][i+1] = tab[j][i];
                tab[j][i] = 0;
            }
            
        }
       
    }
}

void supprLigne(int tab[12][17], int height, int width){
    bool plein = true;
    for (int i = height-1; i>=0; --i){
        for(int j = 0; j < width; ++j){
            if(tab[j][i]==0){
                plein = false;
            }
        }
        if(plein){
            printf("La ligne %d est pleine !\n", i);
            for(int h = 0; h < width; ++h){
                tab[h][i] = 0;
            }
            chuteLigneSuppr(tab, height, width, i);
        }else{
            plein=true;
        }  
    }
    
}



bool chutePossible(int tab[12][17], int height, int width, Tetromino tetro){ 
    
    bool chutePossible = false;


    //recupération des coordonnée des autres case du tetromino
    std::set<std::pair<int, int>> listeCase = tetro.getCases();

    for (auto it = listeCase.begin(); it != listeCase.end(); it++){
        if(it->second < height-1){
            //printf("%d\n", it->second);
            if(tab[it->first][it->second+1] > 0){
                for(std::set<std::pair<int, int>>::const_iterator iter = listeCase.begin(); iter != listeCase.end(); iter++){
                    if(iter->second == it->second+1 && iter->first == it->first){
                        chutePossible = true;
                    }
                    
                }

                if(!chutePossible){
                    return false;
                }
                chutePossible = false;
            }
        }else{
            return false;
        }
    }
    
    
    return true;
}

bool droitePossible(int tab[12][17], int height, int width, Tetromino tetro){
    bool possible = false;


    //recupération des coordonnée des autres case du tetromino
    std::set<std::pair<int, int>> listeCase = tetro.getCases();

    for (auto it = listeCase.begin(); it != listeCase.end(); it++){
        if(it->first < width-1){
            //printf("%d\n", it->second);
            if(tab[it->first+1][it->second] > 0){
                for(std::set<std::pair<int, int>>::const_iterator iter = listeCase.begin(); iter != listeCase.end(); iter++){
                    if(iter->second == it->second && iter->first == it->first+1){
                        possible = true;
                    }
                    
                }

                if(!possible){
                    return false;
                }
                possible = false;
            }
        }else{
            return false;
        }
    }
    
    
    return true;
}

bool gauchePossible(int tab[12][17], int height, int width, Tetromino tetro){
    bool possible = false;


    //recupération des coordonnée des autres case du tetromino
    std::set<std::pair<int, int>> listeCase = tetro.getCases();

    for (auto it = listeCase.begin(); it != listeCase.end(); it++){
        if(it->first >0){
            //printf("%d\n", it->second);
            if(tab[it->first-1][it->second] > 0){
                for(std::set<std::pair<int, int>>::const_iterator iter = listeCase.begin(); iter != listeCase.end(); iter++){
                    if(iter->second == it->second && iter->first == it->first-1){
                        possible = true;
                    }
                    
                }

                if(!possible){
                    return false;
                }
                possible = false;
            }
        }else{
            return false;
        }
    }
    
    
    return true;
}

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
    
///////////////////////////////////////////

    // initialization
        static constexpr int width = 12;
        static constexpr int height = 17;
        static constexpr int sizeCase = 40;

        static constexpr gf::Vector2u ScreenSize(1024, 576);
        static constexpr gf::Vector2f ViewSize(2 * width * sizeCase + sizeCase, height * sizeCase); // dummy values
        static constexpr gf::Vector2f ViewCenter = ViewSize / 2; // dummy values



        gf::Window window("Testris", ScreenSize);
        window.setVerticalSyncEnabled(true);
        window.setFramerateLimit(60);
        gf::RenderWindow renderer(window);

        
        int tabJeu[width][height];
        //init tab
        for (int i = 0; i < height; ++i){
            for (int j = 0; j < width; ++j){
                tabJeu[j][i]=0;
            }
            
        }

        ///////////////////////////////////////////////////////////////
        gf::Image imageVide;
        imageVide.create({ sizeCase, sizeCase }, gf::Color4u{0xFF, 0xFF, 0xFF, 0xFF});

        gf::Texture textureVide;
        if (!textureVide.loadFromImage(imageVide)) {
        return EXIT_FAILURE;
        }

        gf::Texture tabTexturePiece[7];

        gf::Image imagePiece;
        imagePiece.create({ sizeCase, sizeCase }, gf::Color4u{0x00, 0x13, 0x52, 0x58});

        gf::Texture texturePiece1;
        if (!texturePiece1.loadFromImage(imagePiece)) {
        return EXIT_FAILURE;
        }
        tabTexturePiece[0] = std::move(texturePiece1);

        gf::Image imagePiece2;
        imagePiece2.create({ sizeCase, sizeCase }, gf::Color4u{0xFF, 0x62, 0x52, 0xFF});

        gf::Texture texturePiece2;
        if (!texturePiece2.loadFromImage(imagePiece2)) {
        return EXIT_FAILURE;
        }
        tabTexturePiece[1] = std::move(texturePiece2);

        gf::Image imagePiece3;
        imagePiece3.create({ sizeCase, sizeCase }, gf::Color4u{0x00, 0x13, 0x52, 0x58});

        gf::Texture texturePiece3;
        if (!texturePiece3.loadFromImage(imagePiece3)) {
        return EXIT_FAILURE;
        }
        tabTexturePiece[2] = std::move(texturePiece3);

        gf::Image imagePiece4;
        imagePiece4.create({ sizeCase, sizeCase }, gf::Color4u{0x12, 0x13, 0xFF, 0x30});

        gf::Texture texturePiece4;
        if (!texturePiece4.loadFromImage(imagePiece4)) {
        return EXIT_FAILURE;
        }
        tabTexturePiece[3] = std::move(texturePiece4);

        gf::Image imagePiece5;
        imagePiece5.create({ sizeCase, sizeCase }, gf::Color4u{0x30, 0x44, 0xFF, 0x30});

        gf::Texture texturePiece5;
        if (!texturePiece5.loadFromImage(imagePiece5)) {
        return EXIT_FAILURE;
        }
        tabTexturePiece[4] = std::move(texturePiece5);

        gf::Image imagePiece6;
        imagePiece6.create({ sizeCase, sizeCase }, gf::Color4u{0x02, 0x13, 0xFF, 0x78});

        gf::Texture texturePiece6;
        if (!texturePiece6.loadFromImage(imagePiece6)) {
        return EXIT_FAILURE;
        }

        tabTexturePiece[5] = std::move(texturePiece6);

        gf::Image imagePiece7;
        imagePiece7.create({ sizeCase, sizeCase }, gf::Color4u{0x12, 0xFF, 0x1F, 0x50});

        gf::Texture texturePiece7;
        if (!texturePiece7.loadFromImage(imagePiece7)) {
        return EXIT_FAILURE;
        }

        tabTexturePiece[6] = std::move(texturePiece7);
        

        //tableau zone de jeu de sprite
        gf::Sprite tabSprite[width][height];

        for (int i = 0; i < height; ++i){
            for (int j = 0; j < width; ++j){
                gf::Sprite sprite(textureVide);
                sprite.setPosition({j* sizeCase, i* sizeCase});
                tabSprite[j][i] = sprite;
            }
            
        }

        /////////////////////////////////////////////////////////////

        /*
        Zone jeu : extend
        autour : screen
        */
            
       
        bool pieceEnJeu = false;

        // views
        gf::ViewContainer views;
        gf::ExtendView mainView(ViewCenter, ViewSize);
        views.addView(mainView);
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
        renderer.clear(gf::Color::White);
        gf::Clock clock;
        gf::Clock clockChute;
        gf::Time t;
        gf::Time periodChute = gf::seconds(1.0f);

    //////////////////////////////////////////////////////////
        
        Deserializer d;
        Serializer s;
        std::vector<uint8_t> request;
        

        while (window.isOpen()) {


            if (queueServer.poll(msg)) {
                printf("-------------------------------\n");
                d.printData();
                d.setData(msg);
                d.deserialize(next_tetro);
                d.printData();
                printf("-------------------------------\n");
                printf("%d\n", next_tetro.getType());
                printf("%d\n", next_tetro.getRotation());
                d.clear();
                d.printData();
                printf("-------------------------------\n" );
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

            if(!pieceEnJeu){ //?????? ICI
                supprLigne(tabJeu, height, width);
                tetro = next_tetro;
                pieceEnJeu = true;
                tabJeu[tetro.getX()][tetro.getY()] = tetro.getType();
                t = clockChute.restart();

                s.serialize(tetro);

                request = s.getData();
                boost::asio::write(*sock, boost::asio::buffer(request, request.capacity()));
                s.clear();
            }

            //printf("%f\n", t.asSeconds());

            if (rightAction.isActive()) {
                if(droitePossible(tabJeu, height, width, tetro)){
                    tabJeu[tetro.getX()][tetro.getY()] = 0;
                    printf("droite\n");
                    tetro.setX(tetro.getX() + 1);
                    tabJeu[tetro.getX()][tetro.getY()] = tetro.getType();
                    //printZoneJeu(tabJeu, height, width);
                }
                
            } else if (leftAction.isActive()) {
                if (gauchePossible(tabJeu, height, width, tetro)){
                    tabJeu[tetro.getX()][tetro.getY()] = 0;
                    printf("droite\n");
                    tetro.setX(tetro.getX() - 1);
                    tabJeu[tetro.getX()][tetro.getY()] = tetro.getType();
                    //printZoneJeu(tabJeu, height, width);
                }
                
            } else if (rotateAction.isActive()) {
                tetro.rotate();
            } else if (downAction.isActive()) {
            // do something
            } else {
            // do something
            }

            t = clockChute.getElapsedTime();

            if(chutePossible(tabJeu, height, width, tetro)){
                if(t > periodChute){
                    tabJeu[tetro.getX()][tetro.getY()] = 0;
                    t = clockChute.restart();
                    //printf("%f\n", t.asSeconds());
                    //printf("Chute\n");
                    //printf("%f\n", periodChute.asSeconds());
                    tetro.setY(tetro.getY() + 1);
                    tabJeu[tetro.getX()][tetro.getY()] = tetro.getType();
                    printZoneJeu(tabJeu, height, width);
                    
                }
            }else{

                pieceEnJeu = false;
                printEtatJeu(tabJeu, height, width, tetro);
            }

            

            for (int i = 0; i < height; ++i){
                for (int j = 0; j < width; ++j){
                    if(tabJeu[j][i]>0){
                        tabSprite[j][i].setTexture(tabTexturePiece[(tabJeu[j][i])-1], true);
                        
                    } else {
                        tabSprite[j][i].setTexture(textureVide, true);
                    }
                }
                
            }

            std::set<std::pair<int, int>> listeCurrentCase = tetro.getCases();

            for (std::set<std::pair<int, int>>::const_iterator it = listeCurrentCase.begin(); it != listeCurrentCase.end(); it++){
                if(it->second>=0){
                    tabSprite[it->first][it->second].setTexture(tabTexturePiece[tetro.getType()-1], true);
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
            
            for (int i = 0; i < height; ++i){
                for (int j = 0; j < width; ++j){
                    renderer.draw(tabSprite[j][i]);
                }
                
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
