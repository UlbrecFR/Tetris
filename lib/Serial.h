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
		ClientQuit,
		ClientCrash
	};

	ErrorType error;
};

struct CTS_GameOver {
	Tetromino tetro;
};

struct Request_CTS{
	enum Type : uint8_t {
		TYPE_TETROMINO_PLACED,
		TYPE_GAME_OVER,
		TYPE_CLIENT_CONNECTION_LOST
	};

	Type type;

	union {
		CTS_TetrominoPlaced tetroMsg;
		CTS_GameOver gameOverMsg;
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
	uint8_t win;
};

struct Request_STC{
	enum Type : uint8_t {
		TYPE_NEW_TETROMINO,
		TYPE_UPDATE,
		TYPE_UPDATE_OTHER,
		TYPE_GAME_START,
		TYPE_GAME_OVER
	};

	Type type;

	union {
		STC_NewTetromino newTetroMsg;
		STC_UpdateGrid updateMsg;
		STC_UpdateOtherGrid updateOtherMsg;
		STC_GameStart gameStart;
		STC_GameOver gameOver;
	};
};

#endif // SERIAL_H
