#include "GameManager.h"
#include <vector>
#include <opencv2/opencv.hpp>
#include "ChessPiece.h"
#include "Board.h"
using namespace std;
using namespace cv;

int Viewer::plateFace;

int GameManager::ImgtoBoradX(Point P) {
	if (Viewer::plateFace == 0) {
		return (P.x / SIZE - 1);
	}
	else {
		return -(P.x / SIZE - 8);
	}
}

int GameManager::ImgtoBoradY(Point P) {
	if (Viewer::plateFace == 0) {
		return -(P.y / SIZE - 8);
	}
	else {
		return (P.y / SIZE - 1);
	}
}

void GameManager::doMouseCallbackEnd(int event, int x, int y, int flags) {
	if (event == EVENT_LBUTTONUP) {
		if (x >= SIZE * 6.25 && x <= SIZE * 7.75 && y >= SIZE * 5.25 && y <= SIZE * 5.75) {
			status = End;
		}
	}
}

void GameManager::drawAll() {

	viewer.drawBoard();
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < players[j]->OwningPiece.size(); i++) {
			viewer.drawChess(players[j]->OwningPiece[i]);
		}
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
	viewer.drawButton(1, 1, 1);
	imshow("Chess Game", viewer.Screen);
}

void GameManager::mouseCallbackEnd(int event, int x, int y, int flags, void* param) {
	GameManager* self = static_cast<GameManager*>(param);
	self->doMouseCallbackEnd(event, x, y, flags);
}

void GameManager::done() {
	renewBoard();
	Board::board_history.push_back(Board::board);
	check = !Board::board.checkCheck(currentPlayer);
	int opponent;
	if (currentPlayer == 0) {
		currentPlayer = 1;
		opponent = 0;
	}
	else {
		currentPlayer = 0;
		opponent = 1;
	}
	drawAll();
}

void GameManager::doMouseCallbackPromoting(int event, int x, int y, int flags) {
	if (event == EVENT_LBUTTONUP) {
		if (x >= SIZE * 2.8125 && x <= SIZE * 3.8125 && y >= SIZE * 5 && y <= SIZE * 6) {
			players[currentPlayer]->promote(players[currentPlayer]->OwningPiece[pieceNo], Queen);
			logTemp = logTemp + " " + 'Q';
		}
		else if (x >= SIZE * 3.9375 && x <= SIZE * 4.9375 && y >= SIZE * 5 && y <= SIZE * 6) {
			players[currentPlayer]->promote(players[currentPlayer]->OwningPiece[pieceNo], Rook);
			logTemp = logTemp + " " + 'R';
		}
		else if (x >= SIZE * 5.0625 && x <= SIZE * 6.0625 && y >= SIZE * 5 && y <= SIZE * 6) {
			players[currentPlayer]->promote(players[currentPlayer]->OwningPiece[pieceNo], Bishop);
			logTemp = logTemp + " " + 'B';
		}
		else if (x >= SIZE * 6.1875 && x <= SIZE * 7.1875 && y >= SIZE * 5 && y <= SIZE * 6) {
			players[currentPlayer]->promote(players[currentPlayer]->OwningPiece[pieceNo], Knight);
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
			flag = players[currentPlayer]->move(players[currentPlayer]->OwningPiece[pieceNo], clickedX, clickedY, players);
			if (flag == true) {
				players[currentPlayer]->beCastling(*Board::board.boardSituation[players[currentPlayer]->OwningPiece[pieceNo].posX + 1][players[currentPlayer]->OwningPiece[pieceNo].posY], clickedX - 1);
				logTemp = logTemp + "O-O";
			}


		}
		else if (players[currentPlayer]->OwningPiece[pieceNo].type == King && players[currentPlayer]->OwningPiece[pieceNo].posX - clickedX == 2) {
			flag = players[currentPlayer]->move(players[currentPlayer]->OwningPiece[pieceNo], clickedX, clickedY, players);
			if (flag == true) {
				players[currentPlayer]->beCastling(*Board::board.boardSituation[players[currentPlayer]->OwningPiece[pieceNo].posX - 2][players[currentPlayer]->OwningPiece[pieceNo].posY], clickedX + 1);
				logTemp = logTemp + "O-O-O";
			}

		}
		else {
			char captured = 'X';
			if (Board::board.boardSituation[clickedX][clickedY] != nullptr) {
				switch (Board::board.boardSituation[clickedX][clickedY]->type) {
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
			flag = players[currentPlayer]->move(players[currentPlayer]->OwningPiece[pieceNo], clickedX, clickedY, players);
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
		if (players[currentPlayer]->OwningPiece[pieceNo].type == Pawn) {
			if ((currentPlayer == 0 && players[currentPlayer]->OwningPiece[pieceNo].posY == 7) || (currentPlayer == 1 && players[currentPlayer]->OwningPiece[pieceNo].posY == 0)) {
				status = Promoting;
				viewer.drawPromotingTips(currentPlayer);
				imshow("Chess Game", viewer.Screen);
				return;
			}
		}
		status = Standby;
		viewer.drawBoard();
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < players[j]->OwningPiece.size(); i++) {
				viewer.drawChess(players[j]->OwningPiece[i]);
			}
		}
		if (flag) {
			Board::clear_stack();
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
			viewer.drawButton(1, 1, 1);
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
					viewer.drawMovable(Board::board, clickedX, clickedY);
					imshow("Chess Game", viewer.Screen);
					status = Moving;
					pieceNo = i;
					startX = clickedX + '0';
					startY = clickedY + '0';
				}
			}
		}
		else if (PointStart.x >= SIZE * 9.125 && PointStart.x <= SIZE * 9.875 && PointStart.y >= SIZE * 1.5 && PointStart.y <= SIZE * 2) {
			if (Viewer::plateFace == 0) {
				Viewer::plateFace = 1;
			}
			else {
				Viewer::plateFace = 0;
			}
			drawAll();
		}
		else if (PointStart.x >= SIZE * 9.125 && PointStart.x <= SIZE * 9.875 && PointStart.y >= SIZE * 2.5 && PointStart.y <= SIZE * 3) {
			//undo
			if (Board::undo()) {
				players[0]->OwningPiece = Board::return_chess_vector(0);
				players[1]->OwningPiece = Board::return_chess_vector(1);
				viewer.drawBoard();
				for (int j = 0; j < 2; j++) {
					for (int i = 0; i < players[j]->OwningPiece.size(); i++) {
						viewer.drawChess(players[j]->OwningPiece[i]);
					}
				}
				viewer.drawButton(1, 1, 1);
				if (currentPlayer == 0) {
					currentPlayer = 1;
				}
				else {
					currentPlayer = 0;
				}
				viewer.drawTurn(currentPlayer);
				imshow("Chess Game", viewer.Screen);
			}
		}
		else if (PointStart.x >= SIZE * 9.125 && PointStart.x <= SIZE * 9.875 && PointStart.y >= SIZE * 3.5 && PointStart.y <= SIZE * 4) {
			//redo
			if (Board::redo()) {
				players[0]->OwningPiece = Board::return_chess_vector(0);
				players[1]->OwningPiece = Board::return_chess_vector(1);
				viewer.drawBoard();
				for (int j = 0; j < 2; j++) {
					for (int i = 0; i < players[j]->OwningPiece.size(); i++) {
						viewer.drawChess(players[j]->OwningPiece[i]);
					}
				}
				viewer.drawButton(1, 1, 1);
				if (currentPlayer == 0) {
					currentPlayer = 1;
				}
				else {
					currentPlayer = 0;
				}
				viewer.drawTurn(currentPlayer);
				imshow("Chess Game", viewer.Screen);

			}
		}
		else if (PointStart.x >= SIZE * 9.125 && PointStart.x <= SIZE * 9.875 && PointStart.y >= SIZE * 6.5 && PointStart.y <= SIZE * 7) {
			//FF.
			viewer.drawButton(1, 1, 0);
			imshow("Chess Game", viewer.Screen);
			status = Checkmate;
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
			Board::board.boardSituation[i][j] = nullptr;
			for (int k = 0; k < 2; k++) {
				for (int l = 0; l < players[k]->OwningPiece.size(); l++) {
					if (players[k]->OwningPiece[l].posX == i && players[k]->OwningPiece[l].posY == j) {
						Board::board.boardSituation[i][j] = &players[k]->OwningPiece[l];
					}
				}
			}
		}
	}
}

void GameManager::domouseCallbackMenu(int event, int x, int y, int flags) {
	if (event == EVENT_LBUTTONUP) {
		if (x >= SIZE && x <= SIZE * 4.5 && y >= SIZE * 5.5 && y <= SIZE * 6.5) {
			status = NewGame;
		}
		else if (x >= SIZE * 5.5 && x <= SIZE * 9 && y >= SIZE * 5.5 && y <= SIZE * 6.5) {
			status = Continue;
		}
		else if (x >= SIZE && x <= SIZE * 4.5 && y >= SIZE * 7 && y <= SIZE * 8) {
			status = Replay;
		}
		else if (x >= SIZE * 5.5 && x <= SIZE * 9 && y >= SIZE * 7 && y <= SIZE * 8) {
			status = End;
		}
	}
}

void GameManager::mouseCallbackMenu(int event, int x, int y, int flags, void* param) {
	GameManager* self = static_cast<GameManager*>(param);
	self->domouseCallbackMenu(event, x, y, flags);
}

void GameManager::exe() {
	system("cls");
	Board::board = Board::return_now_board();
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
		int player0, player1;
		system("cls");
		cout << "Please Enter White Player's Status. 0 = Human, 1 = AI" << endl;
		cin >> player0;
		cout << "Please Enter Black Player's Status. 0 = Human, 1 = AI" << endl;
		cin >> player1;
		if (player0 == 1 && player1 == 0) {
			Viewer::plateFace = 1;
		}
		else {
			Viewer::plateFace = 0;
		}
		system("cls");
		Board::write_init_board();
		Board::load_board();

		viewer.drawBoard();
		for (int i = 0; i < 2; i++) {
			players[i] = new HumanPlayer(i);
			//這裡可能要多一個是有一方是AI的狀況，目前是兩方都是玩家
		}
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < players[j]->OwningPiece.size(); i++) {
				viewer.drawChess(players[j]->OwningPiece[i]);
			}
		}
		int undoable = 1, redoable = 1;

		viewer.drawButton(undoable, redoable, 1);
		currentPlayer = 0;
		viewer.drawTurn(currentPlayer);
		imshow("Chess Game", viewer.Screen);
		renewBoard();
		status = Standby;
	}
	else if (status == Continue) {
		//todo
		cout << "File's Name. :";
		cin >> filename;
		status = Standby;
	}
	else if (status == Replay) {
		//todo
		cout << "File's Name. :";
		cin >> filename;
		status = Replaying;
	}
	else if (status == End) {
		exit(0);
	}
	while (status == Standby || status == Moving || status == Promoting) {

		switch (status) {
		case Standby:
			setMouseCallback("Chess Game", mouseCallbackStandby, this);
			break;
		case Moving:
			setMouseCallback("Chess Game", mouseCallbackMoving, this);
			break;
		case Promoting:
			setMouseCallback("Chess Game", mouseCallbackPromoting, this);
			break;
		default:
			break;
		}
		waitKey(100);
		tick[currentPlayer]++;
		cout << "White: " << tick[0] / 600 << ":" << (tick[0] % 600) / 10 << " | " << "Black: " << tick[1] / 600 << ":" << (tick[1] % 600) / 10 << '\r';
		if (tick[currentPlayer] == 6000) {
			status = Checkmate;
			viewer.drawButton(1, 1, 0);
			imshow("Chess Game", viewer.Screen);
		}

	}
	while (status == Replaying) {
		//todo
	}
	setMouseCallback("Chess Game", mouseCallbackEnd, this);
	waitKey(3000);
	while (1) {
		switch (status) {
		case Checkmate:
			viewer.drawCheckmate(currentPlayer);
			imshow("Chess Game", viewer.Screen);
			setMouseCallback("Chess Game", mouseCallbackEnd, this);
			break;
		case Stalemate:
			viewer.drawStalemate(currentPlayer);
			imshow("Chess Game", viewer.Screen);
			setMouseCallback("Chess Game", mouseCallbackEnd, this);
			break;
		case End:
			exit(0);
		default:
			break;
		}
		waitKey(100);
	}
}

bool GameManager::stalemate(Player& player) {
	for (int i = 0; i < player.OwningPiece.size(); i++) {
		Board::board.checkMovable(player.OwningPiece[i]);
		if (player.OwningPiece[i].movableX.size() != 0 || player.OwningPiece[i].capturableX.size() != 0) {
			return false;
		}
	}
	return true;
}

bool GameManager::checkmate(Player& player, int opponent) {
	if (stalemate(player) && !Board::board.checkCheck(opponent)) {
		return true;
	}
	else
		return false;
}
