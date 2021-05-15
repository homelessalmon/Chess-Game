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

void ChessPiece::checkMovable() {
	movableX.clear();
	movableY.clear();
	switch (type) {
	case King:
		for (int faceX = -1; faceX <= 1; faceX++) {
			for (int faceY = -1; faceY <= 1; faceY++) {
				if (faceX == 0 && faceY == 0) {
					continue;
				}
				for (int i = 1; i <= 1; i++) {
					int targetX, targetY;
					targetX = posX + i * faceX;
					targetY = posY + i * faceY;
					if (targetX >= 0 && targetX < 8 && targetY >= 0 && targetY < 8) {
						movableX.push_back(targetX);
						movableY.push_back(targetY);
					}
				}
			}
		}
		// todo
		// 入堡
		break;
	case Queen:
		for (int faceX = -1; faceX <= 1; faceX++) {
			for (int faceY = -1; faceY <= 1; faceY++) {
				if (faceX == 0 && faceY == 0) {
					continue;
				}
				for (int i = 1; i <= 8; i++) {
					int targetX, targetY;
					targetX = posX + i * faceX;
					targetY = posY + i * faceY;
					if (targetX >= 0 && targetX < 8 && targetY >= 0 && targetY < 8) {
						movableX.push_back(targetX);
						movableY.push_back(targetY);
					}
				}
			}
		}
		//todo 檢查可動位置是否有棋子
		break;
	case Rook:
		for (int faceX = -1; faceX <= 1; faceX++) {
			for (int faceY = -1; faceY <= 1; faceY++) {
				if (faceX != 0 && faceY != 0 || faceX == 0 && faceY == 0) {
					continue;
				}
				for (int i = 1; i <= 8; i++) {
					int targetX, targetY;
					targetX = posX + i * faceX;
					targetY = posY + i * faceY;
					if (targetX >= 0 && targetX < 8 && targetY >= 0 && targetY < 8) {
						movableX.push_back(targetX);
						movableY.push_back(targetY);
					}
				}
			}
		}
		// todo
		break;
	case Bishop:
		for (int faceX = -1; faceX <= 1; faceX++) {
			for (int faceY = -1; faceY <= 1; faceY++) {
				if (faceX == 0 || faceY == 0) {
					continue;
				}
				for (int i = 1; i <= 8; i++) {
					int targetX, targetY;
					targetX = posX + i * faceX;
					targetY = posY + i * faceY;
					if (targetX >= 0 && targetX < 8 && targetY >= 0 && targetY < 8) {
						movableX.push_back(targetX);
						movableY.push_back(targetY);
					}
				}
			}
		}
		// todo
		break;
	case Knight:
		for (int faceX = -1; faceX <= 1; faceX++) {
			for (int faceY = -1; faceY <= 1; faceY++) {
				if (faceX != 0 && faceY != 0 || faceX == 0 && faceY == 0) {
					continue;
				}
				if (faceX == 1 || faceX == -1) {
					for (int i = -1; i <= 1; i++) {
						if (i == 0) continue;
						int targetX, targetY;
						targetX = posX + 2 * faceX;
						targetY = posY + i * faceX;
						if (targetX >= 0 && targetX < 8 && targetY >= 0 && targetY < 8) {
							movableX.push_back(targetX);
							movableY.push_back(targetY);
						}
					}
				}
				if (faceY == 1 || faceY == -1) {
					for (int i = -1; i <= 1; i++) {
						if (i == 0) continue;
						int targetX, targetY;
						targetX = posX + i * faceY;
						targetY = posY + 2 * faceY;
						if (targetX >= 0 && targetX < 8 && targetY >= 0 && targetY < 8) {
							movableX.push_back(targetX);
							movableY.push_back(targetY);
						}
					}
				}
			}
		}
		// todo
		break;
	case Pawn:
		if (player == 0) {
			for (int i = 1; i <= 2; i++) {
				int targetX, targetY;
				targetX = posX;
				targetY = posY + i;
				if (targetX >= 0 && targetX < 8 && targetY >= 0 && targetY < 8) {
					movableX.push_back(targetX);
					movableY.push_back(targetY);
				}
			}
		}
		if (player == 1) {
			for (int i = 1; i <= 2; i++) {
				int targetX, targetY;
				targetX = posX;
				targetY = posY - i;
				if (targetX >= 0 && targetX < 8 && targetY >= 0 && targetY < 8) {
					movableX.push_back(targetX);
					movableY.push_back(targetY);
				}
			}
		}
		// todo
		// 注意吃法與移動法不同
		// 注意兵過路吃
		break;
	default:
		break;
	}
}

bool ChessPiece::move(int x, int y) {
	checkMovable();
	for (int i = 0; i < movableX.size(); i++) {
		if (movableX[i] == x && movableY[i] == y) {
			posX = x;
			posY = y;
			return true;
		}
		else {
			continue;
		}
	}
	return false;
}
