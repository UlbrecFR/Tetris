#include "Tetromino.h"

	Tetromino::Tetromino(){
		srand(time(NULL));
		rotation = rand()%4;
		type = rand()%7;
	}

	void Tetromino::print(){
		for (size_t i = 0; i < 4; ++i){
			for (size_t j = 0; j < 2; ++j){
				printf("%d ", shape[type][i][j]);
			}
			printf("\n");
		}
	}
	