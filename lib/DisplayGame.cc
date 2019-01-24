#include "DisplayGame.h"

void DisplayGame::loadTextures(){

    if (!tabTexture[0].loadFromFile(gf::Path("../ressources/0.png"))) {
        exit(EXIT_FAILURE);
    }

    if (!tabTexture[1].loadFromFile(gf::Path("../ressources/1.png"))) {
        exit(EXIT_FAILURE);
    }

    if (!tabTexture[2].loadFromFile(gf::Path("../ressources/2.png"))) {
        exit(EXIT_FAILURE);
    }

    if (!tabTexture[3].loadFromFile(gf::Path("../ressources/3.png"))) {
        exit(EXIT_FAILURE);
    }

    if (!tabTexture[4].loadFromFile(gf::Path("../ressources/4.png"))) {
        exit(EXIT_FAILURE);
    }

    if (!tabTexture[5].loadFromFile(gf::Path("../ressources/5.png"))) {
        exit(EXIT_FAILURE);
    }

    if (!tabTexture[6].loadFromFile(gf::Path("../ressources/6.png"))) {
        exit(EXIT_FAILURE);
    }
}

void DisplayGame::draw(Grid & gdSelf, Grid & gdOther, Tetromino & currentTetro, Tetromino & nextTetro, uint32_t score, gf::RenderWindow & renderer, gf::RenderStates & r_state){
	gaSelf.updateTextureBackground(gdSelf);
    gaOther.updateTextureBackground(gdOther);
    gaSelf.updateTextureTetromino(currentTetro);
    gaSelf.draw(renderer, r_state);
    gaOther.draw(renderer, r_state);
    for (size_t i = 0; i < 4; ++i){
    	for (size_t j = 0; j < 2; ++j){
    		if (nextTetro.getShape(i,j) > 0){
    			tabSprite[i][j].setTexture(tabTexture[nextTetro.getType()-1]);
    		} else {
    			tabSprite[i][j].unsetTexture();
    		}
    		renderer.draw(tabSprite[i][j]);
    	}
    }
    scoreText.setPosition({18*SIZE_CASE,13.5*SIZE_CASE});
    scoreText.setString("You : ");
    renderer.draw(scoreText);
    scoreText.setPosition({19*SIZE_CASE,14.5*SIZE_CASE});
    if (score == 0){
        scoreText.setString(std::to_string(score));
    } else {
        scoreText.setString(std::to_string(score) + "00");
    }
    renderer.draw(scoreText);
    scoreText.setPosition({18*SIZE_CASE,15.5*SIZE_CASE});
    scoreText.setString("Other Player : ");
    renderer.draw(scoreText);
    scoreText.setPosition({19*SIZE_CASE,16.5*SIZE_CASE});
    if (score == 0){
        scoreText.setString(std::to_string(score));
    } else {
        scoreText.setString(std::to_string(score) + "00");
    }
    renderer.draw(scoreText);
}

void DisplayGame::drawWait(gf::RenderWindow & renderer){
	renderer.draw(spriteWait);
}

void DisplayGame::drawWinLoose(bool win, gf::RenderWindow & renderer){
	
	if(win){
		spriteGameOver.setTexture(textureWin);
	} else {
		spriteGameOver.setTexture(textureLost);
	}

	renderer.draw(spriteGameOver);
}
