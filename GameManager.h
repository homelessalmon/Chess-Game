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
	// int boardSituation[8][8]; //可能會用到？


	void exe();
	static void mouseCallbackStandby(int event, int x, int y, int flags, void* param);
	static void mouseCallbackMoving(int event, int x, int y, int flags, void* param);
	static void mouseCallbackPromoting(int event, int x, int y, int flags, void* param);
	void doMouseCallbackStandby(int event, int x, int y, int flags);
	void doMouseCallbackMoving(int event, int x, int y, int flags);
	void doMouseCallbackPromoting(int event, int x, int y, int flags);

	void renewBoard();
	static int ImgtoBoradX(Point P); //得出P點對應的棋盤X座標
	static int ImgtoBoradY(Point P); //得出P點對應的棋盤Y座標

private:
	Status status;
	int pieceNo;
#if TEST == 1
	int startX;
	int startY;
	vector<ChessPiece> testPiece;
#endif // TEST == 1
};
