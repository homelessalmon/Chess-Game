#pragma once
#include<vector>
#include<string>
#include<fstream> 
#include<sstream>
#include<stack>
#include"ChessPiece.h"
const string split = "-------------------------------------------------------------------";

class New_Board
{
public:
	static vector<New_Board> board_history;
	static std::stack<New_Board> stack;
	ChessPiece* board[8][8];
	
	New_Board();
	New_Board(const New_Board& B);
	~New_Board();
	void operator=(New_Board);
	bool operator==(New_Board);
	static void load_board();
	static void write_board();
	static void write_init_board();
	static New_Board return_now_board();
	static vector<ChessPiece> return_chess_vector(int _player);
	static void undo();
	static void redo();
};

