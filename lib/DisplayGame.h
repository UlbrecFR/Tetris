#ifndef DISPLAYGAME_H
#define DISPLAYGAME_H

#include <gf/RenderWindow.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>
#include <gf/Action.h>
#include <gf/Sprite.h>
#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/EntityContainer.h>
#include <gf/Event.h>
#include <gf/Text.h>
#include <gf/Font.h>
#include <gf/Shapes.h>

#include "Tetromino.h"
#include "Grid.h"
#include "GameArea.h"

class DisplayGame{

	GameArea gaSelf, gaOther;
	gf::Text scoreText;
	gf::Font font;

	gf::Texture textureWin;
	gf::Texture textureLost;
	gf::Texture textureDraw;
	gf::Texture textureWaitPlayer;
	gf::Texture textureWaitServer;

	gf::Sprite spriteGameOver;
	gf::Sprite spriteWaitPlayer;
	gf::Sprite spriteWaitServer;

	gf::Sprite tabSprite[4][2];

	gf::Texture tabTexture[7];

	gf::Texture textureMalus[2];
	gf::Sprite iconeMalus;
    gf::RectangleShape coolDown;

    gf::Clock clkAnimCoolDown;
    uint8_t nbMalusNext;

	public:
		DisplayGame(){

			loadTextures();
			gaSelf.loadTextures(tabTexture);
			gaOther.loadTextures(tabTexture);

			if (!textureWin.loadFromFile(gf::Path("../ressources/win.png"))) {exit(EXIT_FAILURE);}
			if (!textureLost.loadFromFile(gf::Path("../ressources/loose.png"))) {exit(EXIT_FAILURE);}
			if (!textureDraw.loadFromFile(gf::Path("../ressources/draw.png"))) {exit(EXIT_FAILURE);}
			if (!textureWaitPlayer.loadFromFile(gf::Path("../ressources/waitingForPlayer.png"))) {exit(EXIT_FAILURE);}
			if (!textureWaitServer.loadFromFile(gf::Path("../ressources/waitingForServer.png"))) {exit(EXIT_FAILURE);}

			gaSelf.setPosition({SIZE_CASE,SIZE_CASE});
			gaOther.setPosition({15*SIZE_CASE,SIZE_CASE});
			gaOther.setScale({0.66666,0.66666}); 

	        font.loadFromFile("../ressources/font.ttf");
	        scoreText.setString("Score : \n" + std::to_string(0));
	        scoreText.setFont(font);
	        scoreText.setCharacterSize(10);
	        scoreText.setColor(gf::Color::White);

	        spriteWaitPlayer.setTexture(textureWaitPlayer);	
	        spriteWaitServer.setTexture(textureWaitServer);	

	        iconeMalus.setScale(0.6);
	        coolDown.setSize({150.0f, 150.0f});
	        coolDown.setColor(gf::Color::Red);
	        coolDown.setScale(0.6);


	        for (size_t i = 0; i < 4; ++i){
		    	for (size_t j = 0; j < 2; ++j){
		    		tabSprite[i][j].setPosition({static_cast<float>((15+(j*0.75))*SIZE_CASE),static_cast<float>((13+(i*0.75))*SIZE_CASE)});
		    	}
		    }
		}
		
		void draw(Grid & gdSelf, Grid & gdOther, Tetromino & currentTetro, Tetromino & nextTetro, 
			uint32_t scoreSelf, uint32_t scoreOther, gf::Time time, uint8_t malusSelf, bool malusOther,
			gf::RenderWindow & renderer, gf::RenderStates & r_state);

		void drawWaitPlayer(gf::RenderWindow & renderer);

		void drawWaitServer(gf::RenderWindow & renderer);

		void drawWin(gf::RenderWindow & renderer);

		void drawLoose(gf::RenderWindow & renderer);

		void drawDraw(gf::RenderWindow & renderer);

		void loadTextures();
};

#endif // DISPLAYGAME_H