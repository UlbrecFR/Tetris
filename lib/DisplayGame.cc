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

void DisplayGame::draw(Grid & gdSelf, Grid & gdOther, Tetromino & currentTetro, Tetromino & nextTetro, uint32_t scoreSelf, uint32_t scoreOther, gf::Time time, gf::RenderWindow & renderer, gf::RenderStates & r_state){
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
    scoreText.setPosition({19*SIZE_CASE,14*SIZE_CASE});

    std::string min = std::to_string(static_cast<int>(time.asSeconds())/60);
    std::string sec = std::to_string(static_cast<int>(time.asSeconds())%60);
    if (sec.length() < 2){
        sec = "0" + sec;
    }

    scoreText.setString(min+" : "+sec);
    renderer.draw(scoreText);
    scoreText.setPosition({18*SIZE_CASE,15*SIZE_CASE});
    scoreText.setString("You : ");
    renderer.draw(scoreText);
    scoreText.setPosition({19*SIZE_CASE,16*SIZE_CASE});
    if (scoreSelf == 0){
        scoreText.setString(std::to_string(scoreSelf));
    } else {
        scoreText.setString(std::to_string(scoreSelf) + "00");
    }
    renderer.draw(scoreText);
    scoreText.setPosition({18*SIZE_CASE,17*SIZE_CASE});
    scoreText.setString("Other Player : ");
    renderer.draw(scoreText);
    scoreText.setPosition({19*SIZE_CASE,18*SIZE_CASE});
    if (scoreOther == 0){
        scoreText.setString(std::to_string(scoreOther));
    } else {
        scoreText.setString(std::to_string(scoreOther) + "00");
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

