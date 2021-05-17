#pragma once
#include <vector>
#include <string>
#include "Player.h"
#include "Board.h"
#include "Viewer.h"
#include "ChessPiece.h"
using namespace std;
typedef class ChessPiece ChessPiece;
#define TEST 1

enum Status {
	Menu,
	NewGame,
	Continue,
	Standby,
	Moving,
	Promoting
};

class GameManager {
public:
	Player* players[2];
	int currentPlayer;
	Board board;
	Viewer viewer;
	string logTemp;
	bool check = false;


	void exe();
	static void mouseCallbackMenu(int event, int x, int y, int flags, void* param);
	static void mouseCallbackStandby(int event, int x, int y, int flags, void* param);
	static void mouseCallbackMoving(int event, int x, int y, int flags, void* param);
	static void mouseCallbackPromoting(int event, int x, int y, int flags, void* param);
	void domouseCallbackMenu(int event, int x, int y, int flags);
	void doMouseCallbackStandby(int event, int x, int y, int flags);
	void doMouseCallbackMoving(int event, int x, int y, int flags);
	void doMouseCallbackPromoting(int event, int x, int y, int flags);
	void done();
	void renewBoard();
	static int ImgtoBoradX(Point P); //得出P點對應的棋盤X座標
	static int ImgtoBoradY(Point P); //得出P點對應的棋盤Y座標

private:
	Status status;
	int pieceNo;
#if TEST == 1
	char startX;
	char startY;
	char captured;
	vector<ChessPiece> testPiece;
#endif // TEST == 1
};

void undo(vector<string>& stack, vector<string>& l_log, Board& board, Player** players);
void redo(vector<string>& stack, vector<string>& l_log, Board& board, Player** players);
