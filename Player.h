#pragma once
#include <vector>
#include "ChessPiece.h"
#include<string>

using namespace std;

typedef class ChessPiece ChessPiece;

class Player {
public:
	vector<ChessPiece> OwningPiece;
	Player(int);
};

class AI_player : public Player
{
	AI_player(int);
};
