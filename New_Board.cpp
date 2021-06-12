#include "New_Board.h"

vector<Board> Board::board_history;
std::stack<Board> Board::stack;
Board Board::board = Board::return_now_board();

ChessPiece::ChessPiece(int _player, Type _type, int _posX, int _posY, int _epc, int _epcd, int _moved)
{
	player = _player;
	type = _type;
	posX = _posX;
	posY = _posY;
	epc = _epc;
	epcd = _epcd;
	moved = _moved;
}

Board::Board()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			boardSituation[i][j] = NULL;
		}
	}
}

Board::Board(const Board& B)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			this->boardSituation[i][j] = NULL;
		}
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (B.boardSituation[i][j] != NULL)
			{
				ChessPiece* tmp = new ChessPiece(B.boardSituation[i][j]->player, B.boardSituation[i][j]->type, B.boardSituation[i][j]->posX, B.boardSituation[i][j]->posY, B.boardSituation[i][j]->epc, B.boardSituation[i][j]->epcd, B.boardSituation[i][j]->moved);
				this->boardSituation[i][j] = tmp;
			}
		}
	}
}

Board::~Board()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (boardSituation[i][j] != NULL)
			{
				delete boardSituation[i][j];
			}
		}
	}
}

Board Board::return_now_board()
{
	Board tmp;
	if (board_history.size() < 1)
	{
		return tmp;
	}
	else
	{
		tmp = board_history[board_history.size() - 1];
		return tmp;
	}
}

vector<ChessPiece> Board::return_chess_vector(int _player)
{
	if (!(_player == 0 || _player == 1))return vector<ChessPiece>();
	Board B;
	B = return_now_board();
	vector<ChessPiece> r;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (B.boardSituation[i][j] != NULL && B.boardSituation[i][j]->player == _player)
			{
				r.push_back(*(B.boardSituation[i][j]));
			}
		}
	}
	return r;
}

void Board::undo()
{
	if (board_history.size() < 2)return;
	stack.push(*(board_history.end() - 1));
	board_history.erase(board_history.end() - 1);
	write_board();
}

void Board::redo()
{
	if (stack.size() <= 0)return;
	else
	{
		board_history.push_back(stack.top());
		stack.pop();
		write_board();
	}
}

void Board::operator=(Board B)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			this->boardSituation[i][j] = NULL;
		}
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (B.boardSituation[i][j] != NULL)
			{
				ChessPiece* tmp = new ChessPiece(B.boardSituation[i][j]->player, B.boardSituation[i][j]->type, B.boardSituation[i][j]->posX, B.boardSituation[i][j]->posY, B.boardSituation[i][j]->epc, B.boardSituation[i][j]->epcd, B.boardSituation[i][j]->moved);
				this->boardSituation[i][j] = tmp;
			}
		}
	}
}

bool Board::operator==(Board B)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (this->boardSituation[i][j]->player != B.boardSituation[i][j]->player || this->boardSituation[i][j]->type != B.boardSituation[i][j]->type || this->boardSituation[i][j]->posX != B.boardSituation[i][j]->posX || this->boardSituation[i][j]->posY != B.boardSituation[i][j]->posY || this->boardSituation[i][j]->epc != B.boardSituation[i][j]->epc || this->boardSituation[i][j]->epcd != B.boardSituation[i][j]->epcd || this->boardSituation[i][j]->moved != B.boardSituation[i][j]->moved)
			{
				return false;
			}
		}
	}
	return true;
}

void Board::load_board()
{
	ifstream fin("board_history.txt");
	if (!fin.is_open())
	{
		Board::write_init_board();
	}
	fin.close();

	Board::board_history.clear();
	fin.open("board_history.txt");

	
	while (true)
	{
		Board tmp;
		string catch_string;
		getline(fin, catch_string);
		if (catch_string == "end")break;
		while (catch_string != split)
		{
			int _player, _type, _posX, _posY, _epc, _epcd, _moved;
			Type t;
			stringstream ss;
			ss.str("");
			ss.clear();
			ss << catch_string;
			ss >> _player >> _type>> _posX >> _posY >> _epc >> _epcd >> _moved;
			t = static_cast<Type>(_type);
			ChessPiece* tmp_p = new ChessPiece(_player, t, _posX, _posY, _epc, _epcd, _moved);
			tmp.boardSituation[_posX][_posY] = tmp_p;
			getline(fin, catch_string);
		}
		board_history.push_back(tmp);
	}
}

void Board::write_board()
{
	ofstream fout("board_history.txt", ios::trunc);
	for (int i = 0; i < board_history.size(); i++)
	{
		for (int j = 0; j < 8; j++)
		{
			for (int k = 0; k < 8; k++)
			{
				if (board_history[i].boardSituation[j][k] != NULL)
				{
					fout << board_history[i].boardSituation[j][k]->player << " " << static_cast<int>(board_history[i].boardSituation[j][k]->type) << " " << board_history[i].boardSituation[j][k]->posX << " " << board_history[i].boardSituation[j][k]->posY << " " << board_history[i].boardSituation[j][k]->epc << " " << board_history[i].boardSituation[j][k]->epcd << " " << board_history[i].boardSituation[j][k]->moved << endl;
				}
			}
		}
		fout << split << endl;
	}
	fout << "end";
	fout.close();
}

void Board::write_init_board()
{
	ifstream fin("board_history.txt");
	if (!fin.is_open())
	{
		system("cd.>board_history.txt");
	}
	fin.close();

	ofstream fout("board_history.txt", ios::trunc);
	//white
	for (int i = 0; i < 8; i++)
	{
		fout << "0 5 " + to_string(i) + " 1 0 0 0" << endl;
	}
	for (int i = 0; i < 8; i += 7)
	{
		fout << "0 2 " + to_string(i) + " 0 0 0 0" << endl;
	}
	for (int i = 1; i < 8; i += 5)
	{
		fout << "0 4 " + to_string(i) + " 0 0 0 0" << endl;
	}
	for (int i = 2; i < 8; i += 3)
	{
		fout << "0 3 " + to_string(i) + " 0 0 0 0" << endl;
	}
	fout << "0 1 3 0 0 0 0" << endl << "0 0 4 0 0 0 0" << endl;
	//black
	for (int i = 0; i < 8; i++)
	{
		fout << "1 5 " + to_string(i) + " 6 0 0 0" << endl;
	}
	for (int i = 0; i < 8; i += 7)
	{
		fout << "1 2 " + to_string(i) + " 7 0 0 0" << endl;
	}
	for (int i = 1; i < 8; i += 5)
	{
		fout << "1 4 " + to_string(i) + " 7 0 0 0" << endl;
	}
	for (int i = 2; i < 8; i += 3)
	{
		fout << "1 3 " + to_string(i) + " 7 0 0 0" << endl;
	}
	fout << "1 1 3 7 0 0 0" << endl << "1 0 4 7 0 0 0" << endl;
	fout << split << endl << "end";
	fout.close();
}

