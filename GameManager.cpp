#include "GameManager.h"
#include <vector>
#include <opencv2/opencv.hpp>
#include "ChessPiece.h"
#include "Board.h"
using namespace std;
using namespace cv;

int GameManager::ImgtoBoradX(Point P) {
	return (P.x / SIZE - 1);
}

int GameManager::ImgtoBoradY(Point P) {
	return -(P.y / SIZE - 8);
}

void GameManager::doMouseCallback2(int event, int x, int y, int flags) {
	if (event == EVENT_LBUTTONUP) {
		Point PointStart = Point(x, y);
		int clickedX = GameManager::ImgtoBoradX(PointStart);
		int clickedY = GameManager::ImgtoBoradY(PointStart);
		bool flag = players[currentPlayer]->OwningPiece[pieceNo].move(clickedX, clickedY);
		viewer.drawBoard();
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < players[j]->OwningPiece.size(); i++) {
				viewer.drawChess(players[j]->OwningPiece[i]);
			}
		}
		imshow("Chess Game", viewer.Screen);
		status = 0;
		if (flag) {

			if (currentPlayer == 0) {
				currentPlayer = 1;
			}
			else {
				currentPlayer = 0;
			}
			renewBoard();
		}


	}
}

void GameManager::mouseCallback2(int event, int x, int y, int flags, void* param) {
	GameManager* self = static_cast<GameManager*>(param);
	self->doMouseCallback2(event, x, y, flags);
}

void GameManager::doMouseCallback1(int event, int x, int y, int flags) {
	if (event == EVENT_LBUTTONUP) {
		Point PointStart = Point(x, y);
		int clickedX = ImgtoBoradX(PointStart);
		int clickedY = ImgtoBoradY(PointStart);
		for (int i = 0; i < players[currentPlayer]->OwningPiece.size(); i++) {
			if (players[currentPlayer]->OwningPiece[i].posX == clickedX && players[currentPlayer]->OwningPiece[i].posY == clickedY) {
				viewer.drawMovable(players[currentPlayer]->OwningPiece[i]);
				imshow("Chess Game", viewer.Screen);
				status = 1;
				pieceNo = i;
			}
		}
	}
}



void GameManager::mouseCallback1(int event, int x, int y, int flags, void* param) {
	GameManager* self = static_cast<GameManager*>(param);
	self->doMouseCallback1(event, x, y, flags);
}

void GameManager::renewBoard() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			board.boardSituation[i][j] = nullptr;
			for (int k = 0; k < 2; k++) {
				for (int l = 0; l < players[k]->OwningPiece.size(); l++) {
					if (players[k]->OwningPiece[l].posX == i && players[k]->OwningPiece[l].posY == j) {
						board.boardSituation[i][j] = &players[k]->OwningPiece[l];
					}
				}
			}
		}
	}
}

void GameManager::exe() {
	viewer.drawBoard();
	for (int i = 0; i < 2; i++) {
		players[i] = new Player(i);
		//這裡可能要多一個是有一方是AI的狀況，目前是兩方都是玩家
	}
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < players[j]->OwningPiece.size(); i++) {
			viewer.drawChess(players[j]->OwningPiece[i]);
		}
	}
	namedWindow("Chess Game", WINDOW_AUTOSIZE);
	imshow("Chess Game", viewer.Screen);
	currentPlayer = 0;
	status = 0;
	while (1) {
		switch (status) {
		case 0:
			setMouseCallback("Chess Game", mouseCallback1, this);
			break;
		case 1:
			setMouseCallback("Chess Game", mouseCallback2, this);
			break;
		default:
			break;
		}
		waitKey(100);
	}
}
