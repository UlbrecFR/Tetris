#include "DisplayGame.h"

void DisplayGame::draw(Grid & gdSelf, Grid & gdOther, Tetromino & currentTetro, Tetromino & nextTetro, uint32_t score, gf::RenderWindow & renderer, gf::RenderStates & r_state){
	gaSelf.updateTextureBackground(gdSelf);
    gaOther.updateTextureBackground(gdOther);
    gaSelf.updateTextureTetromino(currentTetro);
    gaSelf.draw(renderer, r_state);
    gaOther.draw(renderer, r_state);
    scoreText.setString("Score : " + std::to_string(score) + "00");
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

