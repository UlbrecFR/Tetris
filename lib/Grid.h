#ifndef GRID_H
#define GRID_H

#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#define WIDTH 12
#define HEIGHT 17

class Grid{

	private:

		uint8_t grid[WIDTH*HEIGHT];

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

		bool isValid(size_t x, size_t y);
};

#endif // GRID_H