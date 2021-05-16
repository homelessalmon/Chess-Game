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
