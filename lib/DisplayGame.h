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

#include "Tetromino.h"
#include "Grid.h"
#include "GameArea.h"

class DisplayGame{

	GameArea gaSelf, gaOther;
	gf::Text scoreText;
	gf::Font font;

	gf::Texture textureWin;
	gf::Texture textureLost;
	gf::Texture textureWait;

	gf::Sprite spriteGameOver;
	gf::Sprite spriteWait;

	gf::Sprite tabSprite[4][2];

	gf::Texture tabTexture[7];

	public:
		DisplayGame(){

			loadTextures();
			gaSelf.loadTextures(tabTexture);
			gaOther.loadTextures(tabTexture);

			if (!textureWin.loadFromFile(gf::Path("../ressources/win.png"))) {exit(EXIT_FAILURE);}
			if (!textureLost.loadFromFile(gf::Path("../ressources/lost.png"))) {exit(EXIT_FAILURE);}
			if (!textureWait.loadFromFile(gf::Path("../ressources/waitingScreen.png"))) {exit(EXIT_FAILURE);}

			gaSelf.setPosition({SIZE_CASE,SIZE_CASE});
			gaOther.setPosition({15*SIZE_CASE,SIZE_CASE});
			gaOther.setScale({0.66666,0.66666}); 

	        font.loadFromFile("../ressources/font.ttf");
	        scoreText.setString("Score : \n" + std::to_string(0));
	        scoreText.setFont(font);
	        scoreText.setCharacterSize(10);
	        scoreText.setColor(gf::Color::White);

	        spriteWait.setTexture(textureWait);	

	        for (size_t i = 0; i < 4; ++i){
		    	for (size_t j = 0; j < 2; ++j){
		    		tabSprite[i][j].setPosition({static_cast<float>((15+j)*SIZE_CASE),static_cast<float>((13+i)*SIZE_CASE)});
		    	}
		    }
		}
		
		void draw(Grid & gdSelf, Grid & gdOther, Tetromino & currentTetro, Tetromino & nextTetro, uint32_t score, gf::RenderWindow & renderer, gf::RenderStates & r_state);

		void drawWait(gf::RenderWindow & renderer);

		void drawWinLoose(bool win, gf::RenderWindow & renderer);

		void loadTextures();
};

#endif // DISPLAYGAME_H