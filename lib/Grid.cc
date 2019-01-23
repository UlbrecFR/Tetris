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

bool Grid::isValid(size_t x, size_t y) const{
	return (x>=0 && x < getCols() && y >= 0 && y < getRows());
}

void Grid::printGrid() const{
	for (size_t row = 0; row < getRows(); ++row){
        for (size_t col = 0; col < getCols(); ++col){
            printf("%d ", (*this)(col,row));
        }
        printf("\n");
    }
    printf("\n");
}

void Grid::addTetromino(Tetromino & t){
	for (auto c : t.getCases()){
        if(c.y>0){
            (*this)(c.x, c.y) = t.getType();
        }
    }
}

void Grid::fallLines(size_t nLine){
	size_t nbrow = getRows();

    for (size_t row = 0; row < getRows(); ++row){
        for (size_t col = 0; col < getCols(); ++col){
            if(isValid(col,nbrow-2-row) && nbrow-row-1<=nLine){
                (*this)(col,nbrow-1-row) = (*this)(col,nbrow-2-row);
            }
        }
    }    
}

uint32_t Grid::deleteLines(){
	bool plein = true;
    uint32_t nbLine = 0;
    for (size_t row = 0; row < getRows(); ++row){
        for (size_t col = 0; col < getCols(); ++col){
            if((*this)(col,row) == 0){
                plein = false;
            }
        }
        if(plein){
            ++nbLine;
            fallLines(row);
            nbLine += deleteLines();
            break;
        } else {
            plein = true;
        }
    }
    return nbLine;
}

bool Grid::movePossible(Tetromino & t, gf::Vector2i dir) const{
	auto cells = t.getCases();
    auto anchor = t.getPos();

    for (auto cell : cells){
        if(!isValid(cell.x+dir.x, cell.y+dir.y)){
            return false;
        }
        if (!((*this)(cell.x+dir.x, cell.y+dir.y) == 0)){
            if (cell+dir != anchor){
                return false;
            } 
        }
    }    
    return true;
}

bool Grid::downPossible(Tetromino & t) const{
	return movePossible(t, {0,1});
}

bool Grid::rightPossible(Tetromino & t) const{
	return movePossible(t, {1,0});
}

bool Grid::leftPossible(Tetromino & t) const{
	return movePossible(t, {-1,0});
}

bool Grid::rotatePossible(Tetromino t) const{
	t.rotate();
    for(auto c : t.getCases()) {
        if (c.x < 0 || c.x >= getCols() || c.y < 0 || c.y >= getRows()) {
            return false;
        }
    }
    return true;
}

bool Grid::gameOver(Tetromino & t) const{
	for (size_t i = 0; i < 4; ++i){
        for (size_t j = 0; j < getCols(); ++j){
            if((*this)(j,i) != 0){
                if(t.getX()!= j && t.getY() != i){
                    return false;
                }
            }
        }
    }
    return true;
}


