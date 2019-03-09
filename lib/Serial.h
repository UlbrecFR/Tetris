#ifndef SERIAL_H
#define SERIAL_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <gf/Array2D.h>
#include "Tetromino.h"
#include "Grid.h"

const static size_t maxLength = 1024;

struct CTS_TetrominoPlaced {
	Tetromino tetro;
};


struct CTS_ClientConnectionLost {
	enum ErrorType : uint8_t {
		TYPE_CLIENT_QUIT,
		TYPE_CLIENT_CRASH
	};

	ErrorType error;
};

struct Request_CTS{
	enum Type : uint8_t {
		TYPE_TETROMINO_PLACED,
		TYPE_CLIENT_CONNECTION_LOST,
	};

	Type type;

	union {
		CTS_TetrominoPlaced tetroMsg;
		CTS_ClientConnectionLost discoMsg;
	};
};




//////////////////////////////////////////////////


struct STC_NewTetromino {
	Tetromino newTetro;
};

struct STC_GameStart {
	Tetromino firstTetro;
	Tetromino secondTetro;
	uint64_t time;
};

struct STC_UpdateGrid {
	Grid grid;
	uint32_t score;
};

struct STC_UpdateOtherGrid {
	Grid grid;
	uint32_t score;
};

struct STC_GameOver {
	enum Results : uint8_t {
		TYPE_WIN,
		TYPE_LOOSE,
		TYPE_DRAW
	};

	Results results;
};

struct STC_MalusStart{
	uint8_t target;
	uint8_t typeMalus;
};

struct STC_MalusEnd{
	uint8_t target;
};

struct Request_STC{
	enum Type : uint8_t {
		TYPE_NEW_TETROMINO,
		TYPE_UPDATE,
		TYPE_UPDATE_OTHER,
		TYPE_GAME_START,
		TYPE_GAME_OVER,
		TYPE_MALUS_START,
		TYPE_MALUS_END,
		TYPE_CONNECTION_LOST,
	};

	Type type;

	union {
		STC_NewTetromino newTetroMsg;
		STC_UpdateGrid updateMsg;
		STC_UpdateOtherGrid updateOtherMsg;
		STC_GameStart gameStart;
		STC_GameOver gameOver;
		STC_MalusStart malusStart;
		STC_MalusEnd malusEnd;
	};
};

#endif // SERIAL_H
