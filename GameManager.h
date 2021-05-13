#pragma once
#include <vector>
#include "Player.h"
#include "Viewer.h"
#include "ChessPiece.h"
using namespace std;
typedef class ChessPiece ChessPiece;
#define TEST 1

class GameManager {
public:
	Player* players[2];
	int current_player;
	Viewer viewer;
	// int boardSituation[8][8]; //可能會用到？


	void exe();
	static void mouseCallback1(int event, int x, int y, int flags, void* param);
	static void mouseCallback2(int event, int x, int y, int flags, void* param);
	void doMouseCallback1(int event, int x, int y, int flags);
	void doMouseCallback2(int event, int x, int y, int flags);

#if TEST == 1
	int status;
	int pieceNo;
	vector<ChessPiece> testPiece;

#endif // TEST == 1
	static int ImgtoBoradX(Point P); //得出P點對應的棋盤X座標
	static int ImgtoBoradY(Point P); //得出P點對應的棋盤Y座標
};
