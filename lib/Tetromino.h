#ifndef TETROMINO_H
#define TETROMINO_H

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <gf/VectorOps.h>

class Tetromino{

	private:
		uint8_t type; //enum
		uint8_t rotation;
		static const uint8_t shape[7][4][2];
		gf::Vector2u pos;

	public:
		Tetromino() = default;
		
		Tetromino(const Tetromino&) = default;
		Tetromino& operator=(const Tetromino& t) = default;

		void print();

		uint8_t getX() const;

		uint8_t getY() const;

		void setX(uint8_t newX);

		void setY(uint8_t newY);

		uint8_t getType() const;

		uint8_t getRotation() const;

		void setType(uint8_t t);

		void setRotation(uint8_t r);

		gf::Vector2u getPos() const;

		void setPos(gf::Vector2u p);

		std::vector<gf::Vector2u> getCases(); //modifier avec Vector2i

		void rotate();

		uint8_t getShape(size_t x, size_t y) const;

};


#endif // TETROMINO_H