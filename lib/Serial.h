#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Tetromino.h"

const static size_t maxLength = 1024;

struct Message {
	char msg[maxLength];
	size_t length;
};


struct CTS_TetrominoPlaced {
	Tetromino tetro;
	int posX;
	int posY;
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

struct STC_UpdateOtherPlayer {
	int grid[12*17];
};

struct Request_CTS{
	enum {
		TYPE_TETROMINO_PLACED,
		TYPE_CLIENT_LOST
	} Type;
	union {
		CTS_TetrominoPlaced tetroMsg;
		CTS_ClientConnectionLost discoMsg;
	} Content;
};

struct Request_STC{
	enum {
		TYPE_NEW_TETROMINO,
		TYPE_UPDATE_OTHER
	} Type;
	union {
		STC_NewTetromino newTetroMsg;
		STC_UpdateOtherPlayer updateOtherMsg;
	} Content;
};
