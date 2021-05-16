#include "Board.h"
//Board::Board() {
//	for (int i = 0; i < 8; i++) {
//		for (int j = 0; j < 8; j++) {
//			boardSituation[i][j] = NULL;
//		}
//	}
//}
//
//Board::Board(Player* players[2]) {
//	for (int i = 0; i < 8; i++) {
//		for (int j = 0; j < 8; j++) {
//			boardSituation[i][j] = NULL;
//		}
//	}
//	for (int i = 0; i < 2; i++) {
//		for (int j = 0; j < players[i]->OwningPiece.size(); j++) {
//			boardSituation[players[i]->OwningPiece[j].posY][players[i]->OwningPiece[j].posX] = &players[i]->OwningPiece[j];
//		}
//	}
//}

void Board::checkMovable(ChessPiece& piece) {
	piece.movableX.clear();
	piece.movableY.clear();
	piece.capturableX.clear();
	piece.capturableY.clear();

	switch (piece.type) {
	case King:
		for (int faceX = -1; faceX <= 1; faceX++) {
			for (int faceY = -1; faceY <= 1; faceY++) {
				if (faceX == 0 && faceY == 0) {
					continue;
				}
				for (int i = 1; i <= 1; i++) {
					int targetX, targetY;
					targetX = piece.posX + i * faceX;
					targetY = piece.posY + i * faceY;
					if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) continue;

					if (boardSituation[targetX][targetY] != NULL) {
						if (boardSituation[targetX][targetY]->player == piece.player)
							break;
						else {
							piece.capturableX.push_back(targetX);
							piece.capturableY.push_back(targetY);
							break;
						}
					}

					if (targetX >= 0 && targetX < 8 && targetY >= 0 && targetY < 8) {
						piece.movableX.push_back(targetX);
						piece.movableY.push_back(targetY);
					}
				}
			}
		}
		// todo
		if (piece.moved == 0) {
			//短
			if (boardSituation[piece.posX + 1][piece.posY] == nullptr && boardSituation[piece.posX + 2][piece.posY] == nullptr && boardSituation[piece.posX + 3][piece.posY]->type == Rook && boardSituation[piece.posX + 3][piece.posY]->moved == 0) {
				piece.movableX.push_back(piece.posX + 2);
				piece.movableY.push_back(piece.posY);
			}
			//長
			if (boardSituation[piece.posX - 1][piece.posY] == nullptr && boardSituation[piece.posX - 2][piece.posY] == nullptr && boardSituation[piece.posX - 3][piece.posY] == nullptr && boardSituation[piece.posX - 4][piece.posY]->type == Rook && boardSituation[piece.posX - 4][piece.posY]->moved == 0) {
				piece.movableX.push_back(piece.posX - 2);
				piece.movableY.push_back(piece.posY);
			}
		}
		break;
	case Queen:
		for (int faceX = -1; faceX <= 1; faceX++) {
			for (int faceY = -1; faceY <= 1; faceY++) {
				if (faceX == 0 && faceY == 0) {
					continue;
				}
				for (int i = 1; i <= 8; i++) {
					int targetX, targetY;
					targetX = piece.posX + i * faceX;
					targetY = piece.posY + i * faceY;
					if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) continue;

					if (boardSituation[targetX][targetY] != NULL) {
						if (boardSituation[targetX][targetY]->player == piece.player)
							break;
						else {
							piece.capturableX.push_back(targetX);
							piece.capturableY.push_back(targetY);
							break;
						}
					}

					if (targetX >= 0 && targetX < 8 && targetY >= 0 && targetY < 8) {
						piece.movableX.push_back(targetX);
						piece.movableY.push_back(targetY);
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
					targetX = piece.posX + i * faceX;
					targetY = piece.posY + i * faceY;
					if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) continue;

					if (boardSituation[targetX][targetY] != NULL) {
						if (boardSituation[targetX][targetY]->player == piece.player)
							break;
						else {
							piece.capturableX.push_back(targetX);
							piece.capturableY.push_back(targetY);
							break;
						}
					}

					if (targetX >= 0 && targetX < 8 && targetY >= 0 && targetY < 8) {
						piece.movableX.push_back(targetX);
						piece.movableY.push_back(targetY);
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
					targetX = piece.posX + i * faceX;
					targetY = piece.posY + i * faceY;
					if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) continue;

					if (boardSituation[targetX][targetY] != NULL) {
						if (boardSituation[targetX][targetY]->player == piece.player)
							break;
						else {
							piece.capturableX.push_back(targetX);
							piece.capturableY.push_back(targetY);
							break;
						}
					}

					if (targetX >= 0 && targetX < 8 && targetY >= 0 && targetY < 8) {
						piece.movableX.push_back(targetX);
						piece.movableY.push_back(targetY);
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
						targetX = piece.posX + 2 * faceX;
						targetY = piece.posY + i * faceX;
						if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) continue;

						if (boardSituation[targetX][targetY] != NULL) {
							if (boardSituation[targetX][targetY]->player == piece.player)
								continue;
							else {
								piece.capturableX.push_back(targetX);
								piece.capturableY.push_back(targetY);
								continue;
							}
						}
						if (targetX >= 0 && targetX < 8 && targetY >= 0 && targetY < 8) {
							piece.movableX.push_back(targetX);
							piece.movableY.push_back(targetY);
						}
					}
				}
				if (faceY == 1 || faceY == -1) {
					for (int i = -1; i <= 1; i++) {
						if (i == 0) continue;
						int targetX, targetY;
						targetX = piece.posX + i * faceY;
						targetY = piece.posY + 2 * faceY;
						if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) continue;

						if (boardSituation[targetX][targetY] != NULL) {
							if (boardSituation[targetX][targetY]->player == piece.player)
								continue;
							else {
								piece.capturableX.push_back(targetX);
								piece.capturableY.push_back(targetY);
								continue;
							}
						}
						else if (targetX >= 0 && targetX < 8 && targetY >= 0 && targetY < 8) {
							piece.movableX.push_back(targetX);
							piece.movableY.push_back(targetY);
						}
					}
				}
			}
		}
		// todo
		break;
	case Pawn:
		int j;
		if (!piece.moved)
			j = 2;
		else
			j = 1;
		if (piece.player == 0) {
			for (int i = 1; i <= j; i++) {
				int targetX, targetY;
				targetX = piece.posX;
				targetY = piece.posY + i;
				if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) continue;
				
				if (boardSituation[targetX][targetY] != NULL) break;

				if (targetX >= 0 && targetX < 8 && targetY >= 0 && targetY < 8) {
					piece.movableX.push_back(targetX);
					piece.movableY.push_back(targetY);
				}
			}

			//check if is capturable
			for (int i = -1; i <= 1; i++) {
				if (i == 0) continue;
				int targetX, targetY;
				targetX = piece.posX + i * 1;
				targetY = piece.posY + 1;
				if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) continue;

				if (boardSituation[targetX][targetY] != NULL) {
					if (boardSituation[targetX][targetY]->player == piece.player)
						continue;
					else {
						piece.capturableX.push_back(targetX);
						piece.capturableY.push_back(targetY);
					}
				}
			}
		}
		if (piece.player == 1) {
			for (int i = 1; i <= j; i++) {
				int targetX, targetY;
				targetX = piece.posX;
				targetY = piece.posY - i;
				if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) continue;
				
				if (boardSituation[targetX][targetY] != NULL) break;

				if (targetX >= 0 && targetX < 8 && targetY >= 0 && targetY < 8) {
					piece.movableX.push_back(targetX);
					piece.movableY.push_back(targetY);
				}
			}

			//check if is capturable
			for (int i = -1; i <= 1; i++) {
				if (i == 0) continue;
				int targetX, targetY;
				targetX = piece.posX + i * 1;
				targetY = piece.posY - 1;
				if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) continue;

				if (boardSituation[targetX][targetY] != NULL) {
					if (boardSituation[targetX][targetY]->player == piece.player)
						continue;
					else {
						piece.capturableX.push_back(targetX);
						piece.capturableY.push_back(targetY);
					}
				}
			}
		}
		// 注意兵過路吃
		break;
	default:
		break;
	}
}

bool Board::move(ChessPiece &piece, int x, int y, Player** players) {
	checkMovable(piece);
	for (int i = 0; i < piece.movableX.size(); i++) {
		if (piece.movableX[i] == x && piece.movableY[i] == y) {
			boardSituation[x][y] = boardSituation[piece.posX][piece.posY];
			boardSituation[piece.posX][piece.posY] = nullptr;
			piece.posX = x;
			piece.posY = y;
			piece.moved++;
			return true;
		}
		else {
			continue;
		}
	}
	for (int i = 0; i < piece.capturableX.size(); i++) {
		int idx;
		if (piece.capturableX[i] == x && piece.capturableY[i] == y) {
			for (idx = 0; idx < players[boardSituation[x][y]->player]->OwningPiece.size(); idx++) {
				if (players[boardSituation[x][y]->player]->OwningPiece[idx].posX == x && players[boardSituation[x][y]->player]->OwningPiece[idx].posY == y) {
					break;
				}
			}
			capture(*players[boardSituation[x][y]->player], idx);
			boardSituation[x][y] = boardSituation[piece.posX][piece.posY];
			boardSituation[piece.posX][piece.posY] = nullptr;
			piece.posX = x;
			piece.posY = y;
			piece.moved++;
			return true;
		}
		else {
			continue;
		}
	}
	return false;
}

void Board::capture(Player &player, int i) {
	player.OwningPiece.erase(player.OwningPiece.begin() + i);
}

void Board::beCastling(ChessPiece& piece, int x) {
	boardSituation[x][piece.posY] = boardSituation[piece.posX][piece.posY];
	boardSituation[piece.posX][piece.posY] = nullptr;
	piece.posX = x;
	piece.moved++;
	return;
}
