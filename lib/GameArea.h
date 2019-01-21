#ifndef GAMEAREA_H
#define GAMEAREA_H

#include <gf/Array2D.h>
#include <gf/Sprite.h>
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
#include <gf/RenderTarget.h>
#include <gf/RenderStates.h>

#include <Tetromino.h>


class GameArea : public gf::Transformable {

	private :
		gf::Array2D<uint8_t, uint8_t> ga;
		gf::Array2D<gf::Sprite, uint8_t> tabSprite;
		gf::Texture tabTexturePiece[7];
		gf::Texture textureVide;

		uint8_t sizeCase;
		uint8_t width;
		uint8_t height;


	public :

		void updateTextureBackground();

		void updateTextureTetromino(Tetromino & tetro);

		void loadTextures();

		void draw(gf::RenderTarget& target, const gf::RenderStates& states) override;

		GameArea(uint8_t width, uint8_t height) :
			width(width), height(height){
			ga({width, height});
			tabSprite({width, height});
			sizeCase = 40;

			loadTextures();

	        for (int i = 0; i < this->height; ++i){
	            for (int j = 0; j < this->width; ++j){
	                gf::Sprite sprite(textureVide);
	                sprite.setPosition({j* sizeCase, i* sizeCase});
	                tabSprite({j, i}) = sprite;
	            }
	            
	        }


		};




};


#endif // GAMEAREA_H