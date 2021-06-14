#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "Player.h"
#include "Viewer.h"
#include "ChessPiece.h"
using namespace std;
typedef class ChessPiece ChessPiece;
#define TEST 0

enum Status {
	Menu,
	NewGame,
	Continue,
	Standby,
	Moving,
	Promoting,
	Checkmate,
	Stalemate,
	End
};

class GameManager {
public:
	Player* players[2];
	int currentPlayer;
	//Board board;
	Viewer viewer;
	string logTemp;
	bool check = false;


	void exe();
	static void mouseCallbackMenu(int event, int x, int y, int flags, void* param);
	static void mouseCallbackStandby(int event, int x, int y, int flags, void* param);
	static void mouseCallbackMoving(int event, int x, int y, int flags, void* param);
	static void mouseCallbackPromoting(int event, int x, int y, int flags, void* param);
	static void mouseCallbackEnd(int event, int x, int y, int flags, void* param);
	void domouseCallbackMenu(int event, int x, int y, int flags);
	void doMouseCallbackStandby(int event, int x, int y, int flags);
	void doMouseCallbackMoving(int event, int x, int y, int flags);
	void doMouseCallbackPromoting(int event, int x, int y, int flags);
	void doMouseCallbackEnd(int event, int x, int y, int flags);
	void drawAll();
	void done();
	void renewBoard();
	static int ImgtoBoradX(Point P); //得出P點對應的棋盤X座標
	static int ImgtoBoradY(Point P); //得出P點對應的棋盤Y座標

	bool stalemate(Player& player);
	bool checkmate(Player& player, int opponent);

private:
	Status status;
	int pieceNo;
	char startX;
	char startY;
	char captured;
#if TEST == 1
	vector<ChessPiece> testPiece;
#endif // TEST == 1
};
