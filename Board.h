#pragma once
#include "ChessPiece.h"

class Board {
public:
	ChessPiece* boardSituation [8][8];
	Board();
	Board(Player* players[2]);
};
