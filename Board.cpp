#include "Board.h"

vector<Board> Board::board_history;
std::stack<Board> Board::stack;
Board Board::board = Board::return_now_board();
int Board::now_player = 0;

Board::Board() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			boardSituation[i][j] = NULL;
		}
	}
}

Board::Board(const Board& B) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			this->boardSituation[i][j] = NULL;
		}
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (B.boardSituation[i][j] != NULL) {
				ChessPiece* tmp = new ChessPiece(B.boardSituation[i][j]->player, B.boardSituation[i][j]->type, B.boardSituation[i][j]->posX, B.boardSituation[i][j]->posY, B.boardSituation[i][j]->epc, B.boardSituation[i][j]->epcd, B.boardSituation[i][j]->moved);
				this->boardSituation[i][j] = tmp;
			}
		}
	}
}

Board::~Board() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (boardSituation[i][j] != NULL) {
				delete boardSituation[i][j];
			}
		}
	}
}

Board Board::return_now_board() {
	Board tmp;
	if (board_history.size() < 1) {
		return tmp;
	}
	else {
		tmp = board_history[board_history.size() - 1];
		return tmp;
	}
}

vector<ChessPiece> Board::return_chess_vector(int _player) {
	if (!(_player == 0 || _player == 1))return vector<ChessPiece>();
	Board B;
	B = return_now_board();
	vector<ChessPiece> r;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (B.boardSituation[i][j] != NULL && B.boardSituation[i][j]->player == _player) {
				r.push_back(*(B.boardSituation[i][j]));
			}
		}
	}
	return r;
}

bool Board::undo() {
	if (board_history.size() < 2)
	{
		return false;
	}
	stack.push(*(board_history.end() - 1));
	board_history.erase(board_history.end() - 1);
	write_board();
	board = return_now_board();
	if (now_player == 0)now_player = 1;
	else now_player = 0;
	return true;
}

bool Board::redo() {
	if (stack.size() <= 0)
	{
		return false;
	}
	else {
		board_history.push_back(stack.top());
		stack.pop();
		write_board();
		board = return_now_board();
		if (now_player == 0)now_player = 1;
		else now_player = 0;
		return false;
	}
}

void Board::operator=(Board B) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			this->boardSituation[i][j] = NULL;
		}
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (B.boardSituation[i][j] != NULL) {
				ChessPiece* tmp = new ChessPiece(B.boardSituation[i][j]->player, B.boardSituation[i][j]->type, B.boardSituation[i][j]->posX, B.boardSituation[i][j]->posY, B.boardSituation[i][j]->epc, B.boardSituation[i][j]->epcd, B.boardSituation[i][j]->moved);
				this->boardSituation[i][j] = tmp;
			}
		}
	}
}

bool Board::operator==(Board B) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (this->boardSituation[i][j]->player != B.boardSituation[i][j]->player || this->boardSituation[i][j]->type != B.boardSituation[i][j]->type || this->boardSituation[i][j]->posX != B.boardSituation[i][j]->posX || this->boardSituation[i][j]->posY != B.boardSituation[i][j]->posY || this->boardSituation[i][j]->epc != B.boardSituation[i][j]->epc || this->boardSituation[i][j]->epcd != B.boardSituation[i][j]->epcd || this->boardSituation[i][j]->moved != B.boardSituation[i][j]->moved) {
				return false;
			}
		}
	}
	return true;
}

void Board::load_board() {
	ifstream fin("board_history.txt");
	if (!fin.is_open()) {
		Board::write_init_board();
	}
	fin.close();

	Board::board_history.clear();
	fin.open("board_history.txt");


	while (true) {
		Board tmp;
		string catch_string;
		getline(fin, catch_string);
		if (catch_string == "end")break;
		while (catch_string != splitLine) {
			int _player, _type, _posX, _posY, _epc, _epcd, _moved;
			Type t;
			stringstream ss;
			ss.str("");
			ss.clear();
			ss << catch_string;
			ss >> _player >> _type >> _posX >> _posY >> _epc >> _epcd >> _moved;
			t = static_cast<Type>(_type);
			ChessPiece* tmp_p = new ChessPiece(_player, t, _posX, _posY, _epc, _epcd, _moved);
			tmp.boardSituation[_posX][_posY] = tmp_p;
			getline(fin, catch_string);
		}
		board_history.push_back(tmp);
		if (now_player == 0)now_player = 1;
		else now_player = 0;
	}
	fin.close();
}

bool Board::specific_load_board(string file_name) {
	Board::board_history.clear();
	ifstream fin(file_name);

	if (fin.is_open()) {
		while (true) {
			Board tmp;
			string catch_string;
			getline(fin, catch_string);
			if (catch_string == "end")break;
			while (catch_string != splitLine) {
				int _player, _type, _posX, _posY, _epc, _epcd, _moved;
				Type t;
				stringstream ss;
				ss.str("");
				ss.clear();
				ss << catch_string;
				ss >> _player >> _type >> _posX >> _posY >> _epc >> _epcd >> _moved;
				t = static_cast<Type>(_type);
				ChessPiece* tmp_p = new ChessPiece(_player, t, _posX, _posY, _epc, _epcd, _moved);
				tmp.boardSituation[_posX][_posY] = tmp_p;
				getline(fin, catch_string);
			}
			board_history.push_back(tmp);
			if (now_player == 0)now_player = 1;
			else now_player = 0;
		}
		fin.close();
		return true;
	}
	else {
		return false;
	}
}

void Board::write_board() {
	ofstream fout("board_history.txt", ios::trunc);
	for (int i = 0; i < board_history.size(); i++) {
		for (int j = 0; j < 8; j++) {
			for (int k = 0; k < 8; k++) {
				if (board_history[i].boardSituation[j][k] != NULL) {
					fout << board_history[i].boardSituation[j][k]->player << " " << static_cast<int>(board_history[i].boardSituation[j][k]->type) << " " << board_history[i].boardSituation[j][k]->posX << " " << board_history[i].boardSituation[j][k]->posY << " " << board_history[i].boardSituation[j][k]->epc << " " << board_history[i].boardSituation[j][k]->epcd << " " << board_history[i].boardSituation[j][k]->moved << endl;
				}
			}
		}
		fout << splitLine << endl;
	}
	fout << "end";
	fout.close();
}

void Board::write_init_board() {
	ifstream fin("board_history.txt");
	if (!fin.is_open()) {
		system("cd.>board_history.txt");
	}
	fin.close();

	ofstream fout("board_history.txt", ios::trunc);
	//white
	for (int i = 0; i < 8; i++) {
		fout << "0 5 " + to_string(i) + " 1 0 0 0" << endl;
	}
	for (int i = 0; i < 8; i += 7) {
		fout << "0 2 " + to_string(i) + " 0 0 0 0" << endl;
	}
	for (int i = 1; i < 8; i += 5) {
		fout << "0 4 " + to_string(i) + " 0 0 0 0" << endl;
	}
	for (int i = 2; i < 8; i += 3) {
		fout << "0 3 " + to_string(i) + " 0 0 0 0" << endl;
	}
	fout << "0 1 3 0 0 0 0" << endl << "0 0 4 0 0 0 0" << endl;
	//black
	for (int i = 0; i < 8; i++) {
		fout << "1 5 " + to_string(i) + " 6 0 0 0" << endl;
	}
	for (int i = 0; i < 8; i += 7) {
		fout << "1 2 " + to_string(i) + " 7 0 0 0" << endl;
	}
	for (int i = 1; i < 8; i += 5) {
		fout << "1 4 " + to_string(i) + " 7 0 0 0" << endl;
	}
	for (int i = 2; i < 8; i += 3) {
		fout << "1 3 " + to_string(i) + " 7 0 0 0" << endl;
	}
	fout << "1 1 3 7 0 0 0" << endl << "1 0 4 7 0 0 0" << endl;
	fout << splitLine << endl << "end";
	fout.close();
}

bool Board::kingCheck(ChessPiece& piece, int opponent, int x, int y) {
	Board tmp;
	ChessPiece* tmp_p = new ChessPiece(piece.player, piece.type, x, y, piece.epc, piece.epcd, piece.moved);
	tmp = *this;
	tmp.boardSituation[piece.posX][piece.posY] = nullptr;
	tmp.boardSituation[x][y] = tmp_p;

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
	tmp = *this;

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
				int targetX, targetY;
				targetX = piece.posX + faceX;
				targetY = piece.posY + faceY;
				if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) continue;
				if (boardSituation[targetX][targetY] != NULL) {
					if (boardSituation[targetX][targetY]->player != piece.player && kingCheck(piece, opponent, targetX, targetY)) {
						piece.capturableX.push_back(targetX);
						piece.capturableY.push_back(targetY);
					}
					continue;
				}
				if (!kingCheck(piece, opponent, targetX, targetY)) continue;
				piece.movableX.push_back(targetX);
				piece.movableY.push_back(targetY);
			}
		}
		if (piece.moved == 0) {
			//µu
			if (boardSituation[piece.posX + 3][piece.posY] != nullptr && boardSituation[piece.posX + 1][piece.posY] == nullptr && boardSituation[piece.posX + 2][piece.posY] == nullptr && boardSituation[piece.posX + 3][piece.posY]->type == Rook && boardSituation[piece.posX + 3][piece.posY]->moved == 0) {
				if (kingCheck(piece, opponent, piece.posX + 2, piece.posY)) {
					piece.movableX.push_back(piece.posX + 2);
					piece.movableY.push_back(piece.posY);
				}
			}
			//ªø
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
				if (faceX != 0) {
					for (int i = -1; i <= 1; i++) {
						if (i == 0) continue;
						int targetX, targetY;
						targetX = piece.posX + 2 * faceX;
						targetY = piece.posY + i * faceX;
						if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) continue;
						if (!kingCheck(piece, opponent, targetX, targetY)) continue;
						if (boardSituation[targetX][targetY] != NULL) {
							if (boardSituation[targetX][targetY]->player != piece.player) {
								piece.capturableX.push_back(targetX);
								piece.capturableY.push_back(targetY);
							}
							continue;
						}
						piece.movableX.push_back(targetX);
						piece.movableY.push_back(targetY);
					}
				}
				else if (faceY != 0) {
					for (int i = -1; i <= 1; i++) {
						if (i == 0) continue;
						int targetX, targetY;
						targetX = piece.posX + i * faceY;
						targetY = piece.posY + 2 * faceY;
						if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) continue;
						if (!kingCheck(piece, opponent, targetX, targetY)) continue;
						if (boardSituation[targetX][targetY] != NULL) {
							if (boardSituation[targetX][targetY]->player != piece.player) {
								piece.capturableX.push_back(targetX);
								piece.capturableY.push_back(targetY);
							}
							continue;
						}
						piece.movableX.push_back(targetX);
						piece.movableY.push_back(targetY);
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
				if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) break;
				if (boardSituation[targetX][targetY] != NULL) {
					break;
				}
				if (!kingCheck(piece, opponent, targetX, targetY)) continue;
				piece.movableX.push_back(targetX);
				piece.movableY.push_back(targetY);
			}

			//check if is capturable
			for (int i = -1; i <= 1; i++) {
				if (i == 0) continue;
				int targetX, targetY;
				targetX = piece.posX + i;
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
					if (boardSituation[targetX][targetY]->player != piece.player && boardSituation[targetX][targetY]->epc && kingCheck(piece, opponent, targetX, targetY + 1)) {
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
				if (targetX < 0 || targetY < 0 || targetX > 7 || targetY > 7) break;
				if (boardSituation[targetX][targetY] != NULL) {
					break;
				}
				if (!kingCheck(piece, opponent, targetX, targetY)) continue;
				piece.movableX.push_back(targetX);
				piece.movableY.push_back(targetY);
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
