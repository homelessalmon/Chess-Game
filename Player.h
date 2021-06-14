#pragma once
#include <vector>
#include <cstdlib>
#include <ctime>
#include "ChessPiece.h"
#include "Board.h"
using namespace std;
typedef class ChessPiece ChessPiece;
typedef class Player player;

class Player {

public:
	vector<ChessPiece> OwningPiece;
	Player(int);
	virtual bool move(ChessPiece& piece, int x, int y, Player**) = 0;
	virtual void promote(ChessPiece& piece, Type type) = 0;
	void beCastling(ChessPiece& piece, int x);
	void capture(Player& player, int i);
};

class HumanPlayer : public Player {

public:
	HumanPlayer(int);
	bool move(ChessPiece& piece, int x, int y, Player**);
	void promote(ChessPiece& piece, Type type);
};

class AIPlayer : public Player {

public:
	AIPlayer(int);
	ChessPiece choosePiece();
	bool move(ChessPiece& piece, int x, int y, Player**);
	void promote(ChessPiece& piece, Type type);
};
