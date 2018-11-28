#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Tetromino.h"

const static size_t maxLength = 1024;

typedef struct Message {
	char msg[maxLength];
	size_t length;
}Message;


typedef struct TetrominoPlaced {
	Tetromino tetro;
	int posX;
	int posY;
} CTS_TetrominoPlaced;


typedef struct ClientConnectionLost {
	enum {
		ClientQuit,
		ClientCrash,
		ClientDisconnected
	} ErrorType;
} CTS_ClientConnectionLost;


typedef struct NewTetromino {
	Tetromino newTetro;
} STC_NewTetromino;

typedef struct UpdateOtherPlayer {
	int grid[8];
} STC_UpdateOtherPlayer;

typedef struct RequestClientToServer{
	enum {
		TYPE_TETROMINO_PLACED,
		TYPE_CLIENT_LOST
	} Type;
	union {
		CTS_TetrominoPlaced tetroMsg;
		CTS_ClientConnectionLost discoMsg;
	} Content;
} Request_CTS;

typedef struct RequestServerToClient{
	enum {
		TYPE_NEW_TETROMINO,
		TYPE_UPDATE_OTHER
	} Type;
	union {
		STC_NewTetromino newTetroMsg;
		STC_UpdateOtherPlayer updateOtherMsg;
	} Content;
} Request_STC;