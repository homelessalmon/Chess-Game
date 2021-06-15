#pragma once
#include <vector>
#include <cstdlib>
#include <ctime>
#include "ChessPiece.h"
#include "Board.h"
using namespace std;

class Player {
public:
	vector<ChessPiece> OwningPiece;
	Player(int);
	virtual bool move(ChessPiece& piece, int x, int y, Player**) = 0;
	virtual void promote(ChessPiece& piece, Type type) = 0;
	virtual int return_player_type() = 0;
	ChessPiece& choosePiece();
	void beCastling(ChessPiece& piece, int x);
	void capture(Player& player, int i);
};

class HumanPlayer : public Player {
public:
	HumanPlayer(int);
	int return_player_type() { return 0; }
	bool move(ChessPiece& piece, int x, int y, Player**);
	void promote(ChessPiece& piece, Type type);
};

class AIPlayer : public Player {
public:
	AIPlayer(int);
	int return_player_type() { return 1; }
	bool move(ChessPiece& piece, int x, int y, Player**);
	void promote(ChessPiece& piece, Type type);
};
