#ifndef SERIAL_H
#define SERIAL_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Tetromino.h"

const static size_t maxLength = 1024;

struct CTS_TetrominoPlaced {
	Tetromino tetro;
	int posX;
	int posY;
};

struct CTS_NextTetrominoPlease {
};


struct CTS_ClientConnectionLost {
	enum {
		ClientQuit,
		ClientCrash,
		ClientDisconnected
	} ErrorType;
};


struct STC_NewTetromino {
	Tetromino newTetro;
};

struct STC_GameStart {
	Tetromino firstTetro;
};

struct STC_UpdateOtherPlayer {
	int grid[12*17];
};

struct Request_CTS{
	enum {
		TYPE_TETROMINO_PLACED,
		TYPE_CLIENT_LOST,
		TYPE_NEXT_TETRO
	} Type;
	union {
		CTS_TetrominoPlaced tetroMsg;
		CTS_ClientConnectionLost discoMsg;
		CTS_NextTetrominoPlease nextTetroMsg;
	} Content;
};

struct Request_STC{
	enum {
		TYPE_NEW_TETROMINO,
		TYPE_UPDATE_OTHER,
		TYPE_GAME_START
	} Type;
	union {
		STC_NewTetromino newTetroMsg;
		STC_UpdateOtherPlayer updateOtherMsg;
		STC_GameStart gameStart;
	} Content;
};

#endif // SERIAL_H
