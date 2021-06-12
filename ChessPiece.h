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
	ChessPiece(int _player, Type _type, int _posX, int _posY, int _epc, int _epcd, int _moved);
	int posX;
	int posY;
	int player; // 0 = 白 1 = 黑
	int moved = 0;
	int epc = 0; //whether en passant capture is available
	int epcd = 0;
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
