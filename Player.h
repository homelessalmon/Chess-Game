#pragma once
#include <vector>
#include "ChessPiece.h"
#include "Board.h"
using namespace std;
typedef class ChessPiece ChessPiece;
typedef class Player player;

class Player {
public:

	vector<ChessPiece> OwningPiece;
	Player(int);
	//virtual void onMove();
	bool move(ChessPiece& piece, int x, int y, Player**);
	void beCastling(ChessPiece& piece, int x);
	void capture(Player& player, int i);
	void promote(ChessPiece& piece, Type type);
};

class AI_player : public Player
{
	AI_player(int);
};

