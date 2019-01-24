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
#include <Grid.h>

#define HEIGHT_G 17
#define WIDTH_G 12
#define SIZE_CASE 40

class GameArea : public gf::Transformable {

	private :
		gf::Sprite tabSprite[HEIGHT_G * WIDTH_G];
		gf::Texture tabTexture[7];
		gf::Texture textureBackground;
        gf::Sprite background;

	public :

		void updateTextureBackground(Grid & gd);

		void updateTextureTetromino(Tetromino & tetro);

		void loadTextures();

		gf::Sprite & operator()(size_t x, size_t y);

		void draw(gf::RenderTarget& target, const gf::RenderStates& states) override;

		void setPosition(gf::Vector2f pos);

		void setScale(gf::Vector2f scale);

		GameArea() {

			loadTextures();

       		background.setTexture(textureBackground);

	        for (size_t i = 0; i < WIDTH_G; ++i){
	        	for (size_t j = 0; j < HEIGHT_G; ++j){
	                gf::Sprite sprite;
	                sprite.setPosition( gf::Vector2f(static_cast<float>(i*SIZE_CASE), static_cast<float>(j*SIZE_CASE)));
	                (*this)(i, j) = sprite;
	            }
	        }
		};
};


#endif // GAMEAREA_H