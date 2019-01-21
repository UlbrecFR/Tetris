#include "GameArea.h"

void GameArea::updateTextureBackground(){

    for (uint8_t i = 0; i < height; ++i){
        for (uint8_t j = 0; j < width; ++j){
            if(ga({j, i}) > 0){
                tabSprite({j,i}).setTexture(tabTexturePiece[(ga({j,i}))-1], true);
                
            } else {
                tabSprite({j, i}).setTexture(textureVide, true);
            }
        }
        
    }

}

void GameArea::updateTextureTetromino(Tetromino & tetro) {
    std::set<std::pair<uint8_t, uint8_t>> listeCurrentCase = tetro.getCases();

	for (auto it : listeCurrentCase){
	    if(it.second>=0){
	        tabSprite({it.first, it.second}).setTexture(tabTexturePiece[tetro.getType()-1], true);
	    }
	    
	}

}


void GameArea::loadTextures(){

    gf::Image imageVide;
    imageVide.create({ sizeCase, sizeCase }, gf::Color4u{0xDF, 0xDF, 0xDF, 0xFF});

    if (!textureVide.loadFromFile(gf::Path("../ressources/caseVide.png"))) {
    	return;
    }

    gf::Image imagePiece;
    imagePiece.create({ sizeCase, sizeCase }, gf::Color4u{0x00, 0x13, 0x52, 0x58});

    gf::Texture texturePiece1;
    if (!texturePiece1.loadFromImage(imagePiece)) {
    	return;
    }
    tabTexturePiece[0] = std::move(texturePiece1);

    gf::Image imagePiece2;
    imagePiece2.create({ sizeCase, sizeCase }, gf::Color4u{0xFF, 0x62, 0x52, 0xFF});

    gf::Texture texturePiece2;
    if (!texturePiece2.loadFromImage(imagePiece2)) {
    	return;
    }
    tabTexturePiece[1] = std::move(texturePiece2);

    gf::Image imagePiece3;
    imagePiece3.create({ sizeCase, sizeCase }, gf::Color4u{0x00, 0x13, 0x52, 0x58});

    gf::Texture texturePiece3;
    if (!texturePiece3.loadFromImage(imagePiece3)) {
    	return;
    }
    tabTexturePiece[2] = std::move(texturePiece3);

    gf::Image imagePiece4;
    imagePiece4.create({ sizeCase, sizeCase }, gf::Color4u{0x12, 0x13, 0xFF, 0x30});

    gf::Texture texturePiece4;
    if (!texturePiece4.loadFromImage(imagePiece4)) {
    	return;
    }
    tabTexturePiece[3] = std::move(texturePiece4);

    gf::Image imagePiece5;
    imagePiece5.create({ sizeCase, sizeCase }, gf::Color4u{0x30, 0x44, 0xFF, 0x30});

    gf::Texture texturePiece5;
    if (!texturePiece5.loadFromImage(imagePiece5)) {
    	return;
    }
    tabTexturePiece[4] = std::move(texturePiece5);

    gf::Image imagePiece6;
    imagePiece6.create({ sizeCase, sizeCase }, gf::Color4u{0x02, 0x13, 0xFF, 0x78});

    gf::Texture texturePiece6;
    if (!texturePiece6.loadFromImage(imagePiece6)) {
    	return;
    }

    tabTexturePiece[5] = std::move(texturePiece6);

    gf::Image imagePiece7;
    imagePiece7.create({ sizeCase, sizeCase }, gf::Color4u{0x12, 0xFF, 0x1F, 0x50});

    gf::Texture texturePiece7;
    if (!texturePiece7.loadFromImage(imagePiece7)) {
   		return;
    }

    tabTexturePiece[6] = std::move(texturePiece7);

}

void GameArea::draw(gf::RenderTarget& target, const gf::RenderStates& states) {
	for (int i = 0; i < height; ++i){
        for (int j = 0; j < width; ++j){
            target.draw(tabSprite({j, i}));
        }
    }
}