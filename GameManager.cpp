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
		if (testPiece[pieceNo].move(clickedX, clickedY)) {
			viewer.drawBoard();
			for (int i = 0; i < testPiece.size(); i++) {
				viewer.drawChess(testPiece[i]);
			}
			imshow("Chess Game", viewer.Screen);
			status = 0;
		}
		else {
			viewer.drawBoard();
			for (int i = 0; i < testPiece.size(); i++) {
				viewer.drawChess(testPiece[i]);
			}
			imshow("Chess Game", viewer.Screen);
			status = 0;
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
		for (int i = 0; i < testPiece.size(); i++) {
			if (testPiece[i].posX == clickedX && testPiece[i].posY == clickedY) {
				viewer.drawMovable(testPiece[i]);
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
	testPiece.clear();
	viewer.drawBoard();
	ChessPiece A(1, Queen, 5, 4);
	testPiece.push_back(A);
	for (int i = 0; i < testPiece.size(); i++) {
		viewer.drawChess(testPiece[i]);
	}
	namedWindow("Chess Game", WINDOW_AUTOSIZE);
	imshow("Chess Game", viewer.Screen);
	status = 0;
	while (1) {
		if (status == 0) {
			setMouseCallback("Chess Game", mouseCallback1, this);
		}
		else {
			setMouseCallback("Chess Game", mouseCallback2, this);
		}
		waitKey(100);
	}
}
