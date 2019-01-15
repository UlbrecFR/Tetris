#ifndef SERIAL_H
#define SERIAL_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <gf/Array2D.h>
#include "Tetromino.h"

const static size_t maxLength = 1024;

struct CTS_TetrominoPlaced {
	Tetromino tetro;
};

struct CTS_NextTetrominoPlease {
};


struct CTS_ClientConnectionLost {
	enum ErrorType : uint8_t {
		ClientQuit,
		ClientCrash,
		ClientDisconnected
	};

	ErrorType error;
};


struct STC_NewTetromino {
	Tetromino newTetro;
};

struct STC_GameStart {
	Tetromino firstTetro;
};

struct STC_UpdateOtherPlayer {
	gf::Array2D<uint8_t, uint8_t> grid;
};

struct Request_CTS{
	enum Type : uint8_t {
		TYPE_TETROMINO_PLACED,
		TYPE_CLIENT_LOST,
		TYPE_NEXT_TETRO
	};

	Type type;

	union {
		CTS_TetrominoPlaced tetroMsg;
		CTS_ClientConnectionLost discoMsg;
		CTS_NextTetrominoPlease nextTetroMsg;
	};
};

struct Request_STC{
	enum Type : uint8_t {
		TYPE_NEW_TETROMINO,
		TYPE_UPDATE_OTHER,
		TYPE_GAME_START
	};

	Type type;

	union {
		STC_NewTetromino newTetroMsg;
		STC_UpdateOtherPlayer updateOtherMsg;
		STC_GameStart gameStart;
	};
};

#endif // SERIAL_H
