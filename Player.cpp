#include "Player.h"

Player::Player(int n) { //n=0 white,1 black
	if (n == 0) {
		ChessPiece P(0, Pawn, 0, 1), Q(0, Queen, 3, 0), K(0, King, 4, 0), R(0, Rook, 0, 0), B(0, Bishop, 2, 0), N(0, Knight, 1, 0);
		for (int i = 0; i < 8; i++) { OwningPiece.push_back(P); P.posX++; }
		for (int i = 0; i < 2; i++) { OwningPiece.push_back(R); R.posX += 7; }
		for (int i = 0; i < 2; i++) { OwningPiece.push_back(N); N.posX += 5; }
		for (int i = 0; i < 2; i++) { OwningPiece.push_back(B); B.posX += 3; }
		OwningPiece.push_back(Q);
		OwningPiece.push_back(K);
	}
	else {
		ChessPiece P(1, Pawn, 0, 6), Q(1, Queen, 3, 7), K(1, King, 4, 7), R(1, Rook, 0, 7), B(1, Bishop, 2, 7), N(1, Knight, 1, 7);
		for (int i = 0; i < 8; i++) { OwningPiece.push_back(P); P.posX++; }
		for (int i = 0; i < 2; i++) { OwningPiece.push_back(R); R.posX += 7; }
		for (int i = 0; i < 2; i++) { OwningPiece.push_back(N); N.posX += 5; }
		for (int i = 0; i < 2; i++) { OwningPiece.push_back(B); B.posX += 3; }
		OwningPiece.push_back(Q);
		OwningPiece.push_back(K);
	}
}
HumanPlayer::HumanPlayer(int n) :Player(n) {};
AIPlayer::AIPlayer(int n) :Player(n) {}

void Player::beCastling(ChessPiece& piece, int x) {
	Board::board.boardSituation[x][piece.posY] = Board::board.boardSituation[piece.posX][piece.posY];
	Board::board.boardSituation[piece.posX][piece.posY] = nullptr;
	piece.posX = x;
	piece.moved++;
	return;
}

void Player::capture(Player& player, int i) {
	player.OwningPiece.erase(player.OwningPiece.begin() + i);
}

bool HumanPlayer::move(ChessPiece& piece, int x, int y, Player** players) {
	Board::board.checkMovable(piece);
	for (int i = 0; i < piece.movableX.size(); i++) {
		if (piece.movableX[i] == x && piece.movableY[i] == y) {
			Board::board.boardSituation[x][y] = Board::board.boardSituation[piece.posX][piece.posY];
			Board::board.boardSituation[piece.posX][piece.posY] = nullptr;
			piece.posX = x;
			piece.posY = y;
			piece.moved++;
			//epc
			if (piece.type == Pawn && piece.player == 0) {
				for (int i = 0; i < piece.epcX.size(); i++) {
					if (piece.epcX[i] == x && piece.epcY[i] == y - 1) {
						int idx;
						for (idx = 0; idx < players[Board::board.boardSituation[x][y - 1]->player]->OwningPiece.size(); idx++) {
							if (players[Board::board.boardSituation[x][y - 1]->player]->OwningPiece[idx].posX == x && players[Board::board.boardSituation[x][y - 1]->player]->OwningPiece[idx].posY == y - 1) {
								break;
							}
						}
						piece.epcd = 1;
						capture(*players[Board::board.boardSituation[x][y - 1]->player], idx);
					}
				}
			}
			if (piece.type == Pawn && piece.player == 1) {
				for (int i = 0; i < piece.epcX.size(); i++) {
					if (piece.epcX[i] == x && piece.epcY[i] == y + 1) {
						int idx;
						for (idx = 0; idx < players[Board::board.boardSituation[x][y + 1]->player]->OwningPiece.size(); idx++) {
							if (players[Board::board.boardSituation[x][y + 1]->player]->OwningPiece[idx].posX == x && players[Board::board.boardSituation[x][y + 1]->player]->OwningPiece[idx].posY == y + 1) {
								break;
							}
						}
						piece.epcd = 1;
						capture(*players[Board::board.boardSituation[x][y + 1]->player], idx);
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
			for (idx = 0; idx < players[Board::board.boardSituation[x][y]->player]->OwningPiece.size(); idx++) {
				if (players[Board::board.boardSituation[x][y]->player]->OwningPiece[idx].posX == x && players[Board::board.boardSituation[x][y]->player]->OwningPiece[idx].posY == y) {
					break;
				}
			}
			capture(*players[Board::board.boardSituation[x][y]->player], idx);

			Board::board.boardSituation[x][y] = Board::board.boardSituation[piece.posX][piece.posY];
			Board::board.boardSituation[piece.posX][piece.posY] = nullptr;
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

void HumanPlayer::promote(ChessPiece& piece, Type type) {
	piece.type = type;
}

ChessPiece& AIPlayer::choosePiece()
{
	vector<int>available;
	for (int i = 0; i < OwningPiece.size(); i++) {
		Board::board.checkMovable(OwningPiece[i]);
		if (OwningPiece[i].capturableX.size() != 0 || OwningPiece[i].capturableX.size() != 0) {
			available.push_back(i);
		}
	}
	int idx = rand() % available.size();
	return OwningPiece[idx];
}

bool AIPlayer::move(ChessPiece& piece, int x, int y, Player** players) {
	Board::board.checkMovable(piece);

	int act, capPos, movePos;
	act = rand() % 2 + 1; //1:move 2:capture
	if (piece.movableX.size() == 0 && piece.capturableX.size() == 0) {
		return false;
	}
	else if (piece.movableX.size() == 0) {
		act == 2;
	}
	else if (piece.capturableX.size() == 0) {
		act == 1;
	}
	int kprev = -999;

	switch (act) {
	case 1:
		movePos = rand() % piece.movableX.size();
		x = piece.movableX[movePos];
		y = piece.movableY[movePos];
		Board::board.boardSituation[x][y] = Board::board.boardSituation[piece.posX][piece.posY];
		Board::board.boardSituation[piece.posX][piece.posY] = nullptr;
		if (piece.type == King && abs(piece.posX - x) == 2) {
			kprev = piece.posX;
		}
		piece.posX = x;
		piece.posY = y;
		piece.moved++;
		if (piece.type == King && abs(kprev - x) == 2) {
			if ((x - kprev) == 2) {
				beCastling(*Board::board.boardSituation[piece.posX + 1][piece.posY], x - 1);
			}
			if ((x - kprev == -2)) {
				beCastling(*Board::board.boardSituation[piece.posX - 2][piece.posY], x + 1);
			}
		}
		//epc
		if (piece.type == Pawn && piece.player == 0) {
			for (int i = 0; i < piece.epcX.size(); i++) {
				if (piece.epcX[i] == x && piece.epcY[i] == y - 1) {
					int idx;
					for (idx = 0; idx < players[Board::board.boardSituation[x][y - 1]->player]->OwningPiece.size(); idx++) {
						if (players[Board::board.boardSituation[x][y - 1]->player]->OwningPiece[idx].posX == x && players[Board::board.boardSituation[x][y - 1]->player]->OwningPiece[idx].posY == y - 1) {
							break;
						}
					}
					piece.epcd = 1;
					capture(*players[Board::board.boardSituation[x][y - 1]->player], idx);
				}
			}
		}
		if (piece.type == Pawn && piece.player == 1) {
			for (int i = 0; i < piece.epcX.size(); i++) {
				if (piece.epcX[i] == x && piece.epcY[i] == y + 1) {
					int idx;
					for (idx = 0; idx < players[Board::board.boardSituation[x][y + 1]->player]->OwningPiece.size(); idx++) {
						if (players[Board::board.boardSituation[x][y + 1]->player]->OwningPiece[idx].posX == x && players[Board::board.boardSituation[x][y + 1]->player]->OwningPiece[idx].posY == y + 1) {
							break;
						}
					}
					piece.epcd = 1;
					capture(*players[Board::board.boardSituation[x][y + 1]->player], idx);
				}
			}
		}

		for (int j = 0; j < players[piece.player]->OwningPiece.size(); j++) {
			players[piece.player]->OwningPiece[j].epc = 0;
		}
		if (piece.type == Pawn && piece.moved == 1)
			piece.epc = 1;
		return true;
	case 2:
		capPos = rand() % piece.capturableX.size();
		x = piece.capturableX[capPos];
		y = piece.capturableY[capPos];
		int idx;
		for (idx = 0; idx < players[Board::board.boardSituation[x][y]->player]->OwningPiece.size(); idx++) {
			if (players[Board::board.boardSituation[x][y]->player]->OwningPiece[idx].posX == x && players[Board::board.boardSituation[x][y]->player]->OwningPiece[idx].posY == y) {
				break;
			}
		}
		capture(*players[Board::board.boardSituation[x][y]->player], idx);

		Board::board.boardSituation[x][y] = Board::board.boardSituation[piece.posX][piece.posY];
		Board::board.boardSituation[piece.posX][piece.posY] = nullptr;
		piece.posX = x;
		piece.posY = y;
		piece.moved++;
		for (int j = 0; j < players[piece.player]->OwningPiece.size(); j++) {
			players[piece.player]->OwningPiece[j].epc = 0;
		}
		return true;
	}
	piece.epcd = 0;
	return false;
}

void AIPlayer::promote(ChessPiece& piece, Type type) {
	int r = rand() % 4;
	switch (r) {
	case 0:
		piece.type = Queen;
		break;
	case 1:
		piece.type = Rook;
		break;
	case 2:
		piece.type = Bishop;
		break;
	case 3:
		piece.type = Knight;
		break;
	}
}
