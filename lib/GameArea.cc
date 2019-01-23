#include "GameArea.h"

void GameArea::updateTextureBackground(Grid & grid){

    for (size_t i = 0; i < WIDTH_G; ++i){
        for (size_t j = 0; j < HEIGHT_G; ++j){
            if(grid(i, j+HEIGHT-HEIGHT_G) > 0){
                (*this)(i,j).setTexture(tabTexture[(grid(i,j+HEIGHT-HEIGHT_G))-1], true);
                
            } else {
                (*this)(i, j).unsetTexture();
            }
        } 
    }
}

void GameArea::updateTextureTetromino(Tetromino & tetro) {
    auto cells = tetro.getCases();
    for (auto cell : cells){
        if(cell.y>=HEIGHT-HEIGHT_G){
            (*this)(cell.x, cell.y-(HEIGHT-HEIGHT_G)).setTexture(tabTexture[tetro.getType()-1], true);
        }    
    }
}


void GameArea::loadTextures(){

    if (!textureBackground.loadFromFile(gf::Path("../ressources/fond.png"))) {
        exit(EXIT_FAILURE);
    }

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


gf::Sprite & GameArea::operator()(size_t x, size_t y) {
    return tabSprite[x*HEIGHT_G + y];
}

void GameArea::draw(gf::RenderTarget& target, const gf::RenderStates& states) {
    target.draw(background);

    for (size_t i = 0; i < WIDTH_G; ++i){
        for (size_t j = 0; j < HEIGHT_G; ++j){
            target.draw((*this)(i, j));
        }
    }
}