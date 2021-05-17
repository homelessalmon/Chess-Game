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
	bool kingCheck(ChessPiece& piece, int opponent, int x, int y);
	bool checkCheck(int opponent);
	bool stalemate(Player& player);
	bool checkmate(Player& player, int opponent);
};
