#pragma once
#include "ChessPiece.h"
#include "Player.h"

class Board {
public:
	ChessPiece* boardSituation [8][8];
	void checkMovable(ChessPiece &piece);
	bool move(ChessPiece &piece, int x, int y, Player** players);
	void beCastling(ChessPiece &piece, int x);
	void capture(Player &player, int i);
	/*Board();
	Board(Player* players[2]);*/
};
