#include <stdlib.h>
#include <time.h>
#include <stdio.h>

class Tetromino{

	private:
		int type;
		int rotation;
		static const int shape[7][4][2];

	public:
		Tetromino();
		
		void print();

};

const int Tetromino::shape[7][4][2] = {
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