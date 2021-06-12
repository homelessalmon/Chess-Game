#include "ChessPiece.h"
#include <opencv2/opencv.hpp>
#include "GameManager.h"
using namespace std;
using namespace cv;
#define TEST 1

ChessPiece::ChessPiece(int player, Type type, int posX, int posY) {
	ChessPiece::player = player;
	ChessPiece::type = type;
	ChessPiece::posX = posX;
	ChessPiece::posY = posY;
}

ChessPiece::ChessPiece(int _player, Type _type, int _posX, int _posY, int _epc, int _epcd, int _moved) {
	player = _player;
	type = _type;
	posX = _posX;
	posY = _posY;
	epc = _epc;
	epcd = _epcd;
	moved = _moved;
}
