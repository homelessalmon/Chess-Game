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
	ChessPiece(int player, Type type, int posX, int posY);
	int posX;
	int posY;
	int player; // 0 = 白 1 = 黑
	int moved = 0;
	int epc = 0; //whether en passant capture is available
	Type type;

	vector<int> movableX;
	vector<int> movableY;
	vector<int> capturableX;
	vector<int> capturableY;
	vector<int> epcX;
	vector<int> epcY;

	//void checkMovable();
	//bool move(int x, int y);
};
