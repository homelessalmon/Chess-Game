#include "ChessPiece.h"
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
		// todo
		// 入堡
		break;
	case Queen:
		for (int i = 1; i <= 8; i++) {
			for (int faceX = -1; faceX <= 1; faceX++) {
				for (int faceY = -1; faceY <= 1; faceY++) {
					if (faceX == 0 && faceY == 0) {
						continue;
					}
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
		// todo
		break;
	case Bishop:
		// todo
		break;
	case Knight:
		// todo
		break;
	case Pawn:
		// todo
		// 注意吃法與移動法不同
		// 注意兵過路吃
		break;
	default:
		break;
	}

}
