#include "Grid.h"

uint8_t * Grid::getArray1D(){
	return grid;
}

uint8_t & Grid::operator()(size_t x, size_t y){
	return grid[x*HEIGHT+y];
}

const uint8_t & Grid::operator()(size_t x, size_t y) const{
	return grid[x*HEIGHT+y];
}

const uint64_t Grid::getDataSize() const{
	return WIDTH*HEIGHT;
}

const size_t Grid::getCols() const{
	return WIDTH;
}

const size_t Grid::getRows() const{
	return HEIGHT;
}

bool Grid::isValid(size_t x, size_t y){
	return (x>=0 && x < WIDTH && y >= 0 && y < HEIGHT);
}