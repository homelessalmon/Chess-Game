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
HumanPlayer::HumanPlayer(int n) :Player(n) {}
AIPlayer::AIPlayer(int n) :Player(n) {}

void Player::beCastling(ChessPiece& piece, int x) {
	Board::board.boardSituation[x][piece.posY] = Board::board.boardSituation[piece.posX][piece.posY];
	Board::board.boardSituation[piece.posX][piece.posY] = nullptr;
	piece.posX = x;
	piece.moved++;
	return;
}

bool Player::move(ChessPiece& piece, int x, int y, Player** players) {
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

void Player::capture(Player& player, int i) {
	player.OwningPiece.erase(player.OwningPiece.begin() + i);
}

void Player::promote(ChessPiece& piece, Type type) {
	piece.type = type;
}
