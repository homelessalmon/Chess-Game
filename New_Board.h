#pragma once
#include<vector>
#include<string>
#include<fstream> 
#include<sstream>
#include<stack>
#include"ChessPiece.h"
const string split = "-------------------------------------------------------------------";

class Board
{
public:
	static Board board;
	static vector<Board> board_history;
	static std::stack<Board> stack;
	ChessPiece* boardSituation[8][8];
	
	Board();
	Board(const Board& B);
	~Board();
	void operator=(Board);
	bool operator==(Board);
	static void load_board();
	static void write_board();
	static void write_init_board();
	static Board return_now_board();
	static vector<ChessPiece> return_chess_vector(int _player);
	static void undo();
	static void redo();
};

