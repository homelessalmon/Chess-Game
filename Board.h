#pragma once
#include "ChessPiece.h"
#include "Player.h"

class Board {
public:
	ChessPiece* boardSituation[8][8];
	void checkMovable(ChessPiece& piece);
	bool move(ChessPiece& piece, int x, int y, Player**);
	void beCastling(ChessPiece& piece, int x);
	void capture(Player& player, int i);
	bool kingCheck(ChessPiece* const [8][8], ChessPiece&, int, int, int);
	bool checkCheck(ChessPiece* const [8][8], ChessPiece&, int);
	/*Board();
	Board(Player* players[2]);*/
};
