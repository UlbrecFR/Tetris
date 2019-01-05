#include "Tetromino.h"


	const uint8_t Tetromino::shape[7][4][2] = {
			{
				{2,0},
				{1,0},
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
				{1,0},
				{1,0},
				{2,1},
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

	Tetromino::Tetromino(){
		srand(time(NULL));
		rotation = 0;
		type = rand()%7+1;
		x = 6;
		y = 0;

	}

	void Tetromino::print(){
		for (size_t i = 0; i < 4; ++i){
			for (size_t j = 0; j < 2; ++j){
				printf("%d ", shape[type-1][i][j]);
			}
			printf("\n");
		}
	}

	uint8_t Tetromino::getX() const{
		return x;
	}

	uint8_t Tetromino::getY() const{
		return y;
	}

	void Tetromino::setX(uint8_t newX){
		x = newX;
	}

	void Tetromino::setY(uint8_t newY){
		y = newY;
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


	std::set<std::pair<int, int>> Tetromino::getCases() {

		std::set<std::pair<int, int>> cases;
		int xAnchor = 0;
		int yAnchor = 0;

		for (int j = 0; j < 4; ++j) {
			for (int i = 0; i < 2; ++i) {
				if (shape[type-1][j][i] == 2) {
					xAnchor = i;
					yAnchor = j;
					break;
				}
			}
		}


		for (int j = 0; j < 4; ++j) {
			for (int i = 0; i < 2; ++i) {
				if (shape[type-1][j][i] != 0) {
					switch(rotation) {
						case 0 :
							cases.insert(std::make_pair(x + (i - xAnchor), y + (j - yAnchor)));
							break;

						case 1 :
							cases.insert(std::make_pair(x - (j - yAnchor), y + (i - xAnchor)));
							break;

						case 2 :
							cases.insert(std::make_pair(x - (i - xAnchor), y - (j - yAnchor)));
							break;

						case 3 :
							cases.insert(std::make_pair(x + (j - yAnchor), y - (i - xAnchor)));
							break;

						default :
							break;
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
