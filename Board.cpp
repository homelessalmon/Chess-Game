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

bool Board::kingCheck(ChessPiece& piece, int opponent, int x, int y) {
	Board tmp;
	for (int j = 0; j < 8; j++) {
		for (int i = 0; i < 8; i++) {
			tmp.boardSituation[i][j] = boardSituation[i][j];
		}
	}
	tmp.boardSituation[piece.posX][piece.posY] = nullptr;
	tmp.boardSituation[x][y] = &piece;

	int kingX = -1, kingY = -1;

	for (int j = 0; j < 8; j++) {
		for (int i = 0; i < 8; i++) {
			if (tmp.boardSituation[i][j] != nullptr) {
				if (tmp.boardSituation[i][j]->player == piece.player && tmp.boardSituation[i][j]->type == King) {
					kingX = i;
					kingY = j;
					break;
				}
			}
		}
		if (kingX != -1) break;
	}

	for (int faceX = -1; faceX <= 1; faceX++) {
		for (int faceY = -1; faceY <= 1; faceY++) {
			if (faceX == 0 && faceY == 0) continue;
			if (faceX != 0 && faceY != 0) {
				for (int i = 1; i <= 8; i++) {
					int targetX, targetY;
					targetX = kingX + i * faceX;
					targetY = kingY + i * faceY;
					if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) break;
					if (tmp.boardSituation[targetX][targetY] != nullptr) {
						if (tmp.boardSituation[targetX][targetY]->player == opponent) {
							if (tmp.boardSituation[targetX][targetY]->type == Bishop || tmp.boardSituation[targetX][targetY]->type == Queen)
								return false;
							if (i == 1 && tmp.boardSituation[targetX][targetY]->type == King)
								return false;
							if (i == 1 && opponent == 1 && faceY == 1 && tmp.boardSituation[targetX][targetY]->type == Pawn)
								return false;
							if (i == 1 && opponent == 0 && faceY == -1 && tmp.boardSituation[targetX][targetY]->type == Pawn)
								return false;
						}
						break;
					}
				}
			}
			else {
				for (int i = 1; i <= 8; i++) {
					int targetX, targetY;
					targetX = kingX + i * faceX;
					targetY = kingY + i * faceY;
					if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) break;
					if (tmp.boardSituation[targetX][targetY] != nullptr) {
						if (tmp.boardSituation[targetX][targetY]->player == opponent) {
							if (tmp.boardSituation[targetX][targetY]->type == Rook || tmp.boardSituation[targetX][targetY]->type == Queen)
								return false;
							if (i == 1 && tmp.boardSituation[targetX][targetY]->type == King)
								return false;
						}
						break;
					}
				}
			}
		}
	}
	//check knight
	for (int faceX = -1; faceX <= 1; faceX++) {
		for (int faceY = -1; faceY <= 1; faceY++) {
			if (faceX != 0 && faceY != 0 || faceX == 0 && faceY == 0) {
				continue;
			}
			if (faceX == 1 || faceX == -1) {
				for (int i = -1; i <= 1; i++) {
					if (i == 0) continue;
					int targetX, targetY;
					targetX = kingX + 2 * faceX;
					targetY = kingY + i * faceX;
					if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) continue;
					if (tmp.boardSituation[targetX][targetY] != nullptr) {
						if (tmp.boardSituation[targetX][targetY]->player == opponent) {
							if (tmp.boardSituation[targetX][targetY]->type == Knight)
								return false;
						}
					}
				}
			}
			if (faceY == 1 || faceY == -1) {
				for (int i = -1; i <= 1; i++) {
					if (i == 0) continue;
					int targetX, targetY;
					targetX = kingX + i * faceY;
					targetY = kingY + 2 * faceY;
					if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) continue;
					if (tmp.boardSituation[targetX][targetY] != nullptr) {
						if (tmp.boardSituation[targetX][targetY]->player == opponent) {
							if (tmp.boardSituation[targetX][targetY]->type == Knight)
								return false;
						}
					}
				}
			}
		}
	}
	return true;
}

bool Board::checkCheck(int opponent) {
	int player;
	if (opponent == 1) {
		player = 0;
	}
	else player = 1;

	Board tmp;
	for (int j = 0; j < 8; j++) {
		for (int i = 0; i < 8; i++) {
			tmp.boardSituation[i][j] = boardSituation[i][j];
		}
	}

	int kingX = -1, kingY = -1;

	for (int j = 0; j < 8; j++) {
		for (int i = 0; i < 8; i++) {
			if (tmp.boardSituation[i][j] != nullptr) {
				if (tmp.boardSituation[i][j]->player == player && tmp.boardSituation[i][j]->type == King) {
					kingX = i;
					kingY = j;
					break;
				}
			}
		}
		if (kingX != -1) break;
	}

	for (int faceX = -1; faceX <= 1; faceX++) {
		for (int faceY = -1; faceY <= 1; faceY++) {
			if (faceX == 0 && faceY == 0) continue;
			if (faceX != 0 && faceY != 0) {
				for (int i = 1; i <= 8; i++) {
					int targetX, targetY;
					targetX = kingX + i * faceX;
					targetY = kingY + i * faceY;
					if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) break;
					if (tmp.boardSituation[targetX][targetY] != nullptr) {
						if (tmp.boardSituation[targetX][targetY]->player == opponent) {
							if (tmp.boardSituation[targetX][targetY]->type == Bishop || tmp.boardSituation[targetX][targetY]->type == Queen)
								return false;
							if (i == 1 && tmp.boardSituation[targetX][targetY]->type == King)
								return false;
							if (i == 1 && opponent == 1 && faceY == 1 && tmp.boardSituation[targetX][targetY]->type == Pawn)
								return false;
							if (i == 1 && opponent == 0 && faceY == -1 && tmp.boardSituation[targetX][targetY]->type == Pawn)
								return false;
						}
						break;
					}
				}
			}
			else {
				for (int i = 1; i <= 8; i++) {
					int targetX, targetY;
					targetX = kingX + i * faceX;
					targetY = kingY + i * faceY;
					if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) break;
					if (tmp.boardSituation[targetX][targetY] != nullptr) {
						if (tmp.boardSituation[targetX][targetY]->player == opponent) {
							if (tmp.boardSituation[targetX][targetY]->type == Rook || tmp.boardSituation[targetX][targetY]->type == Queen)
								return false;
							if (i == 1 && tmp.boardSituation[targetX][targetY]->type == King)
								return false;
						}
						break;
					}
				}
			}
		}
	}
	//check knight
	for (int faceX = -1; faceX <= 1; faceX++) {
		for (int faceY = -1; faceY <= 1; faceY++) {
			if (faceX != 0 && faceY != 0 || faceX == 0 && faceY == 0) {
				continue;
			}
			if (faceX == 1 || faceX == -1) {
				for (int i = -1; i <= 1; i++) {
					if (i == 0) continue;
					int targetX, targetY;
					targetX = kingX + 2 * faceX;
					targetY = kingY + i * faceX;
					if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) continue;
					if (tmp.boardSituation[targetX][targetY] != nullptr) {
						if (tmp.boardSituation[targetX][targetY]->player == opponent) {
							if (tmp.boardSituation[targetX][targetY]->type == Knight)
								return false;
						}
					}
				}
			}
			if (faceY == 1 || faceY == -1) {
				for (int i = -1; i <= 1; i++) {
					if (i == 0) continue;
					int targetX, targetY;
					targetX = kingX + i * faceY;
					targetY = kingY + 2 * faceY;
					if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) continue;
					if (tmp.boardSituation[targetX][targetY] != nullptr) {
						if (tmp.boardSituation[targetX][targetY]->player == opponent) {
							if (tmp.boardSituation[targetX][targetY]->type == Knight)
								return false;
						}
					}
				}
			}
		}
	}
	return true;
}

void Board::checkMovable(ChessPiece& piece) {
	piece.movableX.clear();
	piece.movableY.clear();
	piece.capturableX.clear();
	piece.capturableY.clear();
	piece.epcX.clear();
	piece.epcY.clear();

	int opponent;
	if (piece.player == 0) opponent = 1;
	else opponent = 0;

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
					if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) break;
					if (boardSituation[targetX][targetY] != NULL) {
						if (boardSituation[targetX][targetY]->player != piece.player && kingCheck(piece, opponent, targetX, targetY)) {
							piece.capturableX.push_back(targetX);
							piece.capturableY.push_back(targetY);
						}
						break;
					}
					if (!kingCheck(piece, opponent, targetX, targetY)) continue;
					piece.movableX.push_back(targetX);
					piece.movableY.push_back(targetY);
				}
			}
		}
		// todo
		if (piece.moved == 0) {
			//短
			if (boardSituation[piece.posX + 3][piece.posY] != nullptr && boardSituation[piece.posX + 1][piece.posY] == nullptr && boardSituation[piece.posX + 2][piece.posY] == nullptr && boardSituation[piece.posX + 3][piece.posY]->type == Rook && boardSituation[piece.posX + 3][piece.posY]->moved == 0) {
				if (kingCheck(piece, opponent, piece.posX + 2, piece.posY)) {
					piece.movableX.push_back(piece.posX + 2);
					piece.movableY.push_back(piece.posY);
				}
			}
			//長
			if (boardSituation[piece.posX - 4][piece.posY] != nullptr && boardSituation[piece.posX - 1][piece.posY] == nullptr && boardSituation[piece.posX - 2][piece.posY] == nullptr && boardSituation[piece.posX - 3][piece.posY] == nullptr && boardSituation[piece.posX - 4][piece.posY]->type == Rook && boardSituation[piece.posX - 4][piece.posY]->moved == 0) {
				if (kingCheck(piece, opponent, piece.posX - 2, piece.posY)) {
					piece.movableX.push_back(piece.posX - 2);
					piece.movableY.push_back(piece.posY);
				}
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
					if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) break;
					if (boardSituation[targetX][targetY] != NULL) {
						if (boardSituation[targetX][targetY]->player != piece.player && kingCheck(piece, opponent, targetX, targetY)) {
							piece.capturableX.push_back(targetX);
							piece.capturableY.push_back(targetY);
						}
						break;
					}
					if (!kingCheck(piece, opponent, targetX, targetY)) continue;
					piece.movableX.push_back(targetX);
					piece.movableY.push_back(targetY);
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
					if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) break;
					if (boardSituation[targetX][targetY] != NULL) {
						if (boardSituation[targetX][targetY]->player != piece.player && kingCheck(piece, opponent, targetX, targetY)) {
							piece.capturableX.push_back(targetX);
							piece.capturableY.push_back(targetY);
						}
						break;
					}
					if (!kingCheck(piece, opponent, targetX, targetY)) continue;
					piece.movableX.push_back(targetX);
					piece.movableY.push_back(targetY);
				}
			}
		}
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
					if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) break;
					if (boardSituation[targetX][targetY] != NULL) {
						if (boardSituation[targetX][targetY]->player != piece.player && kingCheck(piece, opponent, targetX, targetY)) {
							piece.capturableX.push_back(targetX);
							piece.capturableY.push_back(targetY);
						}
						break;
					}
					if (!kingCheck(piece, opponent, targetX, targetY)) continue;
					piece.movableX.push_back(targetX);
					piece.movableY.push_back(targetY);
				}
			}
		}
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
							if (boardSituation[targetX][targetY]->player != piece.player && kingCheck(piece, opponent, targetX, targetY)) {
								piece.capturableX.push_back(targetX);
								piece.capturableY.push_back(targetY);
							}
							continue;
						}
						if (!kingCheck(piece, opponent, targetX, targetY)) continue;
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
							if (boardSituation[targetX][targetY]->player != piece.player && kingCheck(piece, opponent, targetX, targetY)) {
								piece.capturableX.push_back(targetX);
								piece.capturableY.push_back(targetY);
							}
							continue;
						}
						if (!kingCheck(piece, opponent, targetX, targetY)) continue;
						if (targetX >= 0 && targetX < 8 && targetY >= 0 && targetY < 8) {
							piece.movableX.push_back(targetX);
							piece.movableY.push_back(targetY);
						}
					}
				}
			}
		}
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
				if (!kingCheck(piece, opponent, targetX, targetY)) continue;
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
					if (boardSituation[targetX][targetY]->player != piece.player && kingCheck(piece, opponent, targetX, targetY)) {
						piece.capturableX.push_back(targetX);
						piece.capturableY.push_back(targetY);
					}
					continue;
				}
			}
			for (int i = -1; i <= 1; i++) { //check epc
				if (i == 0) continue;
				int targetX, targetY;
				targetX = piece.posX + i * 1;
				targetY = piece.posY;
				if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) continue;

				if (boardSituation[targetX][targetY] != NULL) {
					if (boardSituation[targetX][targetY]->player != piece.player && boardSituation[targetX][targetY]->epc &&kingCheck(piece, opponent, targetX, targetY + 1)) {
						piece.epcX.push_back(targetX);
						piece.epcY.push_back(targetY);
						piece.movableX.push_back(targetX);
						piece.movableY.push_back(targetY + 1);
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
				if (!kingCheck(piece, opponent, targetX, targetY)) continue;
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
					if (boardSituation[targetX][targetY]->player != piece.player && kingCheck(piece, opponent, targetX, targetY)) {
						piece.capturableX.push_back(targetX);
						piece.capturableY.push_back(targetY);
					}
					continue;
				}
			}
			for (int i = -1; i <= 1; i++) {
				if (i == 0) continue;
				int targetX, targetY;
				targetX = piece.posX + i * 1;
				targetY = piece.posY;
				if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) continue;

				if (boardSituation[targetX][targetY] != NULL) {
					if (boardSituation[targetX][targetY]->player != piece.player && boardSituation[targetX][targetY]->epc && kingCheck(piece, opponent, targetX, targetY - 1)) {
						piece.epcX.push_back(targetX);
						piece.epcY.push_back(targetY);
						piece.movableX.push_back(targetX);
						piece.movableY.push_back(targetY - 1);
					}
				}
			}
		}
		break;
	default:
		break;
	}
}

bool Board::move(ChessPiece& piece, int x, int y, Player** players) {
	checkMovable(piece);
	for (int i = 0; i < piece.movableX.size(); i++) {
		if (piece.movableX[i] == x && piece.movableY[i] == y) {
			boardSituation[x][y] = boardSituation[piece.posX][piece.posY];
			boardSituation[piece.posX][piece.posY] = nullptr;
			piece.posX = x;
			piece.posY = y;
			piece.moved++;
			//epc
			if (piece.type == Pawn && piece.player == 0) {
				for (int i = 0; i < piece.epcX.size(); i++) {
					if (piece.epcX[i] == x && piece.epcY[i] == y - 1) {
						int idx;
						for (idx = 0; idx < players[boardSituation[x][y - 1]->player]->OwningPiece.size(); idx++) {
							if (players[boardSituation[x][y - 1]->player]->OwningPiece[idx].posX == x && players[boardSituation[x][y - 1]->player]->OwningPiece[idx].posY == y - 1) {
								break;
							}
						}
						piece.epcd = 1;
						capture(*players[boardSituation[x][y - 1]->player], idx);
					}
				}
			}
			if (piece.type == Pawn && piece.player == 1) {
				for (int i = 0; i < piece.epcX.size(); i++) {
					if (piece.epcX[i] == x && piece.epcY[i] == y + 1) {
						int idx;
						for (idx = 0; idx < players[boardSituation[x][y + 1]->player]->OwningPiece.size(); idx++) {
							if (players[boardSituation[x][y + 1]->player]->OwningPiece[idx].posX == x && players[boardSituation[x][y + 1]->player]->OwningPiece[idx].posY == y + 1) {
								break;
							}
						}
						piece.epcd = 1;
						capture(*players[boardSituation[x][y + 1]->player], idx);
					}
				}
			}

			for (int j = 0; j < players[piece.player]->OwningPiece.size(); j++) {
				players[piece.player]->OwningPiece[j].epc = 0;
			}
			if (piece.type == Pawn && piece.moved == 1)
				piece.epc = 1;
			return true;
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
			for (int j = 0; j < players[piece.player]->OwningPiece.size(); j++) {
				players[piece.player]->OwningPiece[j].epc = 0;
			}
			return true;
		}
	}
	piece.epcd = 0;
	return false;
}

void Board::capture(Player& player, int i) {
	player.OwningPiece.erase(player.OwningPiece.begin() + i);
}

void Board::beCastling(ChessPiece& piece, int x) {
	boardSituation[x][piece.posY] = boardSituation[piece.posX][piece.posY];
	boardSituation[piece.posX][piece.posY] = nullptr;
	piece.posX = x;
	piece.moved++;
	return;
}

bool Board::stalemate(Player& player) {
	for (int i = 0; i < player.OwningPiece.size(); i++) {
		checkMovable(player.OwningPiece[i]);
		if (player.OwningPiece[i].movableX.size() != 0 || player.OwningPiece[i].capturableX.size() != 0) {
			return false;
		}
	}
	return true;
}

bool Board::checkmate(Player& player, int opponent) {
	if (stalemate(player) && !checkCheck(opponent)) {
		return true;
	}
	else
		return false;
}
