#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stack>
#include "ChessPiece.h"
const string splitLine = "-------------------------------------------------------------------";

class Board {
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
	static void load_board(int& player1, int& time1, int& player2, int& time2);
	static bool specific_load_board(string file_name, int& player1, int& time1, int& player2, int& time2);
	static void write_board(int player1, int time1, int player2, int time2);
	static void write_init_board();
	static void specific_write_board(string file_name, int player1, int time1, int player2, int time2);
	static Board return_now_board();
	static vector<ChessPiece> return_chess_vector(int _player);
	static bool undo();
	static bool redo();
	static void clear_stack();

	void checkMovable(ChessPiece& piece);
	bool kingCheck(ChessPiece& piece, int opponent, int x, int y);
	bool checkCheck(int opponent);
};
