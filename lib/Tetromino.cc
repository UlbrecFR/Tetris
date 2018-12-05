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

	std::set<std::pair<int, int>> Tetromino::getCases(int x, int y) {

		std::set<std::pair<int, int>> cases;
		int xAnchor = 0;
		int yAnchor = 0;

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 2; ++j) {
				if (shape[type][i][j] != 0) {
					if (shape[type][i][j] == 2) {
						xAnchor = j;
						yAnchor = i;

						std::set<std::pair<int, int>> cases_n;
						for (auto it = cases.begin(); it != cases.end(); it++) {
							cases_n.insert(std::make_pair(it->first - xAnchor, it->second - yAnchor));
						}
						cases.swap(cases_n);
						cases.insert(std::make_pair(x, y));
					} else {

						cases.insert(std::make_pair(j + x - xAnchor, i + y - yAnchor));
					}

				}
			}
		}

		return cases;
	}