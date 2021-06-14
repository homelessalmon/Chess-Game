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
	virtual bool move(ChessPiece& piece, int x, int y, Player**);
	virtual void promote(ChessPiece& piece, Type type);
	void beCastling(ChessPiece& piece, int x);
	void capture(Player& player, int i);
};

class HumanPlayer : public Player
{
	HumanPlayer(int);
	bool move(ChessPiece& piece, int x, int y, Player**) override;
	void promote(ChessPiece& piece, Type type) override;
};

class AIPlayer : public Player
{
	AIPlayer(int);
	//bool move(ChessPiece& piece, int x, int y, Player**) override;
	//void promote(ChessPiece& piece, Type type) override;
};
