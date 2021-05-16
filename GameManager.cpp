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

void GameManager::doMouseCallbackPromoting(int event, int x, int y, int flags) {
	if (event == EVENT_LBUTTONUP) {
	}
}

void GameManager::mouseCallbackPromoting(int event, int x, int y, int flags, void* param) {
	GameManager* self = static_cast<GameManager*>(param);
	self->doMouseCallbackPromoting(event, x, y, flags);
}

void GameManager::doMouseCallbackMoving(int event, int x, int y, int flags) {
	if (event == EVENT_LBUTTONUP) {
		Point PointStart = Point(x, y);
		int clickedX = GameManager::ImgtoBoradX(PointStart);
		int clickedY = GameManager::ImgtoBoradY(PointStart);
		bool flag;
		logTemp = "";
		char p = currentPlayer + '0';
		logTemp = logTemp + p + " ";
		if (players[currentPlayer]->OwningPiece[pieceNo].type == King && players[currentPlayer]->OwningPiece[pieceNo].posX - clickedX == -2) {
			flag = board.move(players[currentPlayer]->OwningPiece[pieceNo], clickedX, clickedY, players);
			if (flag == true) {
				board.beCastling(*board.boardSituation[players[currentPlayer]->OwningPiece[pieceNo].posX + 1][players[currentPlayer]->OwningPiece[pieceNo].posY], clickedX - 1);
				logTemp = logTemp + "O-O";
			}


		}
		else if (players[currentPlayer]->OwningPiece[pieceNo].type == King && players[currentPlayer]->OwningPiece[pieceNo].posX - clickedX == 2) {
			flag = board.move(players[currentPlayer]->OwningPiece[pieceNo], clickedX, clickedY, players);
			if (flag == true) {
				board.beCastling(*board.boardSituation[players[currentPlayer]->OwningPiece[pieceNo].posX - 2][players[currentPlayer]->OwningPiece[pieceNo].posY], clickedX + 1);
				logTemp = logTemp + "O-O-O";
			}

		}
		else {
			char captured = 'X';
			if (board.boardSituation[clickedX][clickedY] != nullptr) {
				switch (board.boardSituation[clickedX][clickedY]->type) {
				case Queen:
					captured = 'Q';
					break;
				case Bishop:
					captured = 'B';
					break;
				case Knight:
					captured = 'K';
					break;
				case Rook:
					captured = 'R';
					break;
				case Pawn:
					captured = 'P';
					break;
				default:
					break;
				}
			}
			flag = board.move(players[currentPlayer]->OwningPiece[pieceNo], clickedX, clickedY, players);
			if (flag) {
				char endX = clickedX + '0';
				char endY = clickedY + '0';
				logTemp = logTemp + startX + " " + startY + " " + endX + " " + endY + " " + captured;
				if (players[currentPlayer]->OwningPiece[pieceNo].epcd == 1) {
					logTemp = logTemp + " " + 'P';
					players[currentPlayer]->OwningPiece[pieceNo].epcd = 0;
				}
			}
		}
		viewer.drawBoard();
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < players[j]->OwningPiece.size(); i++) {
				viewer.drawChess(players[j]->OwningPiece[i]);
			}
		}
		imshow("Chess Game", viewer.Screen);
		if (players[currentPlayer]->OwningPiece[pieceNo].type == Pawn) {
			if (currentPlayer == 0 && players[currentPlayer]->OwningPiece[pieceNo].posY == 7) {
				status = Promoting;
				viewer.drawPromotingTips(currentPlayer);
				imshow("Chess Game", viewer.Screen);
				return;
			}
		}
		status = Standby;
		if (flag) {
			cout << logTemp << endl;
			renewBoard();
			if (currentPlayer == 0) {
				currentPlayer = 1;
			}
			else {
				currentPlayer = 0;
				viewer.drawTurn(0);
			}
		}

		viewer.drawTurn(currentPlayer);
		imshow("Chess Game", viewer.Screen);

	}
}

void GameManager::mouseCallbackMoving(int event, int x, int y, int flags, void* param) {
	GameManager* self = static_cast<GameManager*>(param);
	self->doMouseCallbackMoving(event, x, y, flags);
}

void GameManager::doMouseCallbackStandby(int event, int x, int y, int flags) {
	if (event == EVENT_LBUTTONUP) {
		Point PointStart = Point(x, y);
		if (PointStart.x >= SIZE && PointStart.x <= SIZE * 9 && PointStart.y >= SIZE && PointStart.y <= SIZE * 9) {
			int clickedX = ImgtoBoradX(PointStart);
			int clickedY = ImgtoBoradY(PointStart);

			for (int i = 0; i < players[currentPlayer]->OwningPiece.size(); i++) {
				if (players[currentPlayer]->OwningPiece[i].posX == clickedX && players[currentPlayer]->OwningPiece[i].posY == clickedY) {
					viewer.drawMovable(board, clickedX, clickedY);
					imshow("Chess Game", viewer.Screen);
					status = Moving;
					pieceNo = i;
					startX = clickedX + '0';
					startY = clickedY + '0';
				}
			}
		}
		else if (PointStart.x >= SIZE * 9.125 && PointStart.x <= SIZE * 9.875 && PointStart.y >= SIZE * 2.5 && PointStart.y <= SIZE * 3) {
			//undo
			viewer.drawBoard();
			for (int j = 0; j < 2; j++) {
				for (int i = 0; i < players[j]->OwningPiece.size(); i++) {
					viewer.drawChess(players[j]->OwningPiece[i]);
				}
			}
			imshow("Chess Game", viewer.Screen);
		}
		else if (PointStart.x >= SIZE * 9.125 && PointStart.x <= SIZE * 9.875 && PointStart.y >= SIZE * 3.5 && PointStart.y <= SIZE * 4) {
			//redo
			viewer.drawBoard();
			for (int j = 0; j < 2; j++) {
				for (int i = 0; i < players[j]->OwningPiece.size(); i++) {
					viewer.drawChess(players[j]->OwningPiece[i]);
				}
			}
			imshow("Chess Game", viewer.Screen);
		}
		else if (PointStart.x >= SIZE * 9.125 && PointStart.x <= SIZE * 9.875 && PointStart.y >= SIZE * 6.5 && PointStart.y <= SIZE * 7) {
			//FF.
		}
	}
}



void GameManager::mouseCallbackStandby(int event, int x, int y, int flags, void* param) {
	GameManager* self = static_cast<GameManager*>(param);
	self->doMouseCallbackStandby(event, x, y, flags);
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
	int undoable = 0, redoable = 0;
	viewer.drawButton(undoable, redoable);
	currentPlayer = 0;
	viewer.drawTurn(currentPlayer);
	namedWindow("Chess Game", WINDOW_AUTOSIZE);
	imshow("Chess Game", viewer.Screen);
	renewBoard();
	status = Standby;
	while (1) {
		switch (status) {
		case Standby:
			setMouseCallback("Chess Game", mouseCallbackStandby, this);
			break;
		case Moving:
			setMouseCallback("Chess Game", mouseCallbackMoving, this);
			break;
		case Promoting:
			setMouseCallback("Chess Game", mouseCallbackPromoting, this);
		default:
			break;
		}
		waitKey(100);
	}
}
