#pragma once
#include <vector>

using namespace std;

enum Type {
	King,
	Queen,
	Rook,
	Bishop,
	Knight,
	Pawn
};

class ChessPiece {
public:
	ChessPiece(int player, Type type,int posX, int posY);
	int posX;
	int posY;
	int player; // 0 = 白 1 = 黑
	Type type;
	// static int boardSituation[8][8]; //可能會用到？
	vector<int> movableX;
	vector<int> movableY;

	void checkMovable();
};
