#include "Tetromino.h"


	const uint8_t Tetromino::shape[7][4][2] = {
			{
				{1,0},
				{2,0},
				{1,0},
				{1,0}
			},

			{
				{2,1},
				{1,0},
				{1,0},
				{0,0}
			},

			{
				{1,2},
				{0,1},
				{0,1},
				{0,0}
			},

			{
				{2,1},
				{1,1},
				{0,0},
				{0,0}
			},

			{
				{1,0},
				{2,1},
				{0,1},
				{0,0}
			},

			{
				{1,0},
				{2,1},
				{1,0},
				{0,0}
			},

			{
				{0,1},
				{1,2},
				{1,0},
				{0,0}
			}
	};

/*
	Tetromino::Tetromino(){
		rotation = 0;
		type = 0;
		pos.x = 6;
		pos.y = 1;

	}
*/

/*
	Tetromino& Tetromino::operator=(const Tetromino& t) {
		rotation = t.rotation;
		type = t.type;
		pos.x = t.pos.x;
		pos.y = t.pos.y;

		return *this;
	}
*/


	void Tetromino::print(){
		for (size_t i = 0; i < 4; ++i){
			for (size_t j = 0; j < 2; ++j){
				printf("%d ", shape[type-1][i][j]);
			}
			printf("\n");
		}
	}

	uint8_t Tetromino::getX() const{
		return pos.x;
	}

	uint8_t Tetromino::getY() const{
		return pos.y;
	}

	void Tetromino::setX(uint8_t newX){
		pos.x = newX;
	}

	void Tetromino::setY(uint8_t newY){
		pos.y = newY;
	}

	uint8_t Tetromino::getType() const{
		return type;
	}
		
	uint8_t Tetromino::getRotation() const{
		return rotation;
	}

	void Tetromino::setType(uint8_t t){
		type = t;
	}

	void Tetromino::setRotation(uint8_t r){
		rotation = r;
	}

	gf::Vector2u Tetromino::getPos() const{
		return pos;
	}

	void Tetromino::setPos(gf::Vector2u p){
		pos = p;
	}

	std::vector<gf::Vector2u> Tetromino::getCases() {

		std::vector<gf::Vector2u> cases;
		uint8_t xAnchor = 0;
		uint8_t yAnchor = 0;

		for (uint8_t j = 0; j < 4; ++j) {
			for (uint8_t i = 0; i < 2; ++i) {
				if (shape[type-1][j][i] == 2) {
					xAnchor = i;
					yAnchor = j;
					break;
				}
			}
		}


		for (uint8_t j = 0; j < 4; ++j) {
			for (uint8_t i = 0; i < 2; ++i) {
				if (shape[type-1][j][i] != 0) {
					if (type != 4) {
						switch(rotation) {
							case 0 :
								cases.push_back({pos.x + (i - xAnchor), pos.y + (j - yAnchor)});
								break;

							case 1 :
								cases.push_back({pos.x - (j - yAnchor), pos.y + (i - xAnchor)});
								break;

							case 2 :
								cases.push_back({pos.x - (i - xAnchor), pos.y - (j - yAnchor)});
								break;

							case 3 :
								cases.push_back({pos.x + (j - yAnchor), pos.y - (i - xAnchor)});
								break;

							default :
								break;
						}
					} else {
						cases.push_back({pos.x + (i - xAnchor), pos.y + (j - yAnchor)});
					}
					
				}
			}
		}

		return cases;
	}

	
	void Tetromino::rotate() {
		rotation++;

		if(rotation == 4) {
			rotation = 0;
		}
	}

	uint8_t Tetromino::getShape(size_t x, size_t y) const{
		return Tetromino::shape[type-1][x][y];
	}
