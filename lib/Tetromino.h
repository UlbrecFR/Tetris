#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <set>

class Tetromino{

	private:
		uint8_t type;
		uint8_t rotation;
		static const uint8_t shape[7][4][2];

	public:
		Tetromino();
		
		void print();

		uint8_t getType() const;
		uint8_t getRotation() const;
		void setType(uint8_t t);
		void setRotation(uint8_t r);
		std::set<std::pair<int, int>> getCases(int x, int y);
};

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