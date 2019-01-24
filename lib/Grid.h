#ifndef GRID_H
#define GRID_H

#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Tetromino.h"

#define WIDTH 12
#define HEIGHT 21

class Grid{

	private:

		uint8_t grid[WIDTH*HEIGHT];

		void fallLines(size_t nLine);

		bool movePossible(Tetromino & t, gf::Vector2i dir) const;

	public:

		Grid() = default;
		
		Grid(const Grid&) = default;
		Grid& operator=(const Grid& t) = default;

		uint8_t *getArray1D();

		uint8_t & operator()(size_t x, size_t y);

		const uint8_t & operator()(size_t x, size_t y) const;

		const uint64_t getDataSize() const;

		const size_t getCols() const;

		const size_t getRows() const;

		bool isValid(size_t x, size_t y) const;

		void printGrid() const;

		void addTetromino(Tetromino & t);

		uint32_t deleteLines();

		bool downPossible(Tetromino & t) const;

		bool rightPossible(Tetromino & t) const;

		bool leftPossible(Tetromino & t) const;

		bool rotatePossible(Tetromino tetro) const;

		bool gameOver(Tetromino & t) const;

		void clear();

};

#endif // GRID_H