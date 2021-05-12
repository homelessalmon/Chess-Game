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
	void exe();

#if TEST == 1
	vector<ChessPiece> testPiece;

#endif // TEST == 1

};
