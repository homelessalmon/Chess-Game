#pragma once
#include <vector>
#include "ChessPiece.h"
#include<string>

using namespace std;

typedef class ChessPiece ChessPiece;

class Player {
	vector<ChessPiece> OwningPiece;
public:
	Player(int);
};

class AI_player : public Player
{

};
