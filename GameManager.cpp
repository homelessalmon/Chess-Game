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

void GameManager::done() {
	cout << logTemp << endl; //這個是log
	renewBoard();
	viewer.drawBoard();
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < players[j]->OwningPiece.size(); i++) {
			viewer.drawChess(players[j]->OwningPiece[i]);
		}
	}
	check = !board.checkCheck(currentPlayer);

	if (currentPlayer == 0) {
		currentPlayer = 1;
	}
	else {
		currentPlayer = 0;
	}
	if (check) {
		for (int i = 0; i < players[currentPlayer]->OwningPiece.size(); i++) {
			if (players[currentPlayer]->OwningPiece[i].type == King) {
				viewer.drawCheck(players[currentPlayer]->OwningPiece[i]);
				break;
			}
		}
	}
	viewer.drawTurn(currentPlayer);
	viewer.drawButton(1, 1);
	imshow("Chess Game", viewer.Screen);
}

void GameManager::doMouseCallbackPromoting(int event, int x, int y, int flags) {
	if (event == EVENT_LBUTTONUP) {
		if (x >= SIZE * 2.8125 && x <= SIZE * 3.8125 && y >= SIZE * 5 && y <= SIZE * 6) {
			players[currentPlayer]->OwningPiece[pieceNo].type = Queen;
			logTemp = logTemp + " " + 'Q';
		}
		else if (x >= SIZE * 3.9375 && x <= SIZE * 4.9375 && y >= SIZE * 5 && y <= SIZE * 6) {
			players[currentPlayer]->OwningPiece[pieceNo].type = Rook;
			logTemp = logTemp + " " + 'R';
		}
		else if (x >= SIZE * 5.0625 && x <= SIZE * 6.0625 && y >= SIZE * 5 && y <= SIZE * 6) {
			players[currentPlayer]->OwningPiece[pieceNo].type = Bishop;
			logTemp = logTemp + " " + 'B';
		}
		else if (x >= SIZE * 6.1875 && x <= SIZE * 7.1875 && y >= SIZE * 5 && y <= SIZE * 6) {
			players[currentPlayer]->OwningPiece[pieceNo].type = Knight;
			logTemp = logTemp + " " + 'K';
		}
		else {
			return;
		}
		status = Standby;
		done();
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
		if (players[currentPlayer]->OwningPiece[pieceNo].type == Pawn) {
			if ((currentPlayer == 0 && players[currentPlayer]->OwningPiece[pieceNo].posY == 7) || (currentPlayer == 1 && players[currentPlayer]->OwningPiece[pieceNo].posY == 0)) {
				status = Promoting;
				viewer.drawPromotingTips(currentPlayer);
				imshow("Chess Game", viewer.Screen);
				return;
			}
		}
		status = Standby;
		if (flag) {
			done();
		}
		else {
			if (check) {
				for (int i = 0; i < players[currentPlayer]->OwningPiece.size(); i++) {
					if (players[currentPlayer]->OwningPiece[i].type == King) {
						viewer.drawCheck(players[currentPlayer]->OwningPiece[i]);
						break;
					}
				}
			}
			viewer.drawTurn(currentPlayer);
			viewer.drawButton(1, 1);
		}
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

void GameManager::domouseCallbackMenu(int event, int x, int y, int flags) {
	if (event == EVENT_LBUTTONUP) {
		if (x >= SIZE && x <= SIZE * 4.5 && y >= SIZE * 7 && y <= SIZE * 8) {
			status = NewGame;
		}
		// else if
		else {
			//
		}
	}
}

void GameManager::mouseCallbackMenu(int event, int x, int y, int flags, void* param) {
	GameManager* self = static_cast<GameManager*>(param);
	self->domouseCallbackMenu(event, x, y, flags);
}

void GameManager::exe() {
	namedWindow("Chess Game", WINDOW_AUTOSIZE);
	viewer.drawMenu();
	imshow("Chess Game", viewer.Screen);
	while (1) {
		setMouseCallback("Chess Game", mouseCallbackMenu, this);
		waitKey(100);
		if (status != Menu) {
			break;
		}
	}
	if (status == NewGame) {
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
		imshow("Chess Game", viewer.Screen);
		renewBoard();
	}
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
