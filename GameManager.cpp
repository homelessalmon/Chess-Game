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

void GameManager::doMouseCallbackEnd(int event, int x, int y, int flags) {
	if (event == EVENT_LBUTTONUP) {
		if (x >= SIZE * 6.25 && x <= SIZE * 7.75 && y >= SIZE * 5.25 && y <= SIZE * 5.75) {
			status = End;
		}
	}
}

void GameManager::mouseCallbackEnd(int event, int x, int y, int flags, void* param) {
	GameManager* self = static_cast<GameManager*>(param);
	self->doMouseCallbackEnd(event, x, y, flags);
}

void GameManager::done() {
	l_log.push_back(logTemp); //這個是log
	log.open("log.txt", ios::out || ios::trunc);
	for (int i = 0; i < l_log.size(); i++)
	{
		log << l_log[i] << endl;
	}
	l_log.clear();
	log.flush(); log.close();//這個是希望每動一次就flush

	renewBoard();
	viewer.drawBoard();
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < players[j]->OwningPiece.size(); i++) {
			viewer.drawChess(players[j]->OwningPiece[i]);
		}
	}
	check = !board.checkCheck(currentPlayer);
	int opponent;
	if (currentPlayer == 0) {
		currentPlayer = 1;
		opponent = 0;
	}
	else {
		currentPlayer = 0;
		opponent = 1;
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
	if (board.checkmate(*players[currentPlayer], opponent)) {
		status = Checkmate;
		viewer.drawButton(1, 0, 0);
	}
	else if (board.stalemate(*players[currentPlayer])) {
		status = Stalemate;
		viewer.drawButton(1, 0, 0);
	}
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
			undo(stack, l_log, board, players);
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
			redo(stack, l_log, board, players);
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
	//file in
	log.open("log.txt", ios::in || ios::out); bool need_load = true; string tmp_string;
	if (!log.is_open())
	{
		system("cd.>log.txt");
		log.open("log", ios::out || ios::in);
		need_load = false;
	}
	else if (log.tellp() == 0)
	{
		need_load = false;
	}
	while (getline(log,tmp_string)) { l_log.push_back(tmp_string); }
	log.close();


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
		viewer.drawButton(undoable, redoable, 1);
		currentPlayer = 0;
		viewer.drawTurn(currentPlayer);
		imshow("Chess Game", viewer.Screen);
		renewBoard();
	}
	status = Standby;
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
			return;
		default:
			break;
		}
		waitKey(100);
	}

}

void undo(vector<string>& stack, vector<string>& l_log, Board& board, Player** players)
{
	if (stack.size() < 1) { return; }
	if (l_log.size() < 1) { return; }
	stack.push_back(l_log[l_log.size() - 1]);
	l_log.pop_back();
	string undo_command = stack[stack.size() - 1].substr(2);
	if (undo_command == "O-O")
	{
		if ((stack[stack.size() - 1])[0] == '0')
		{
			board.boardSituation[0][6]->posX = 4;
			board.boardSituation[0][4] = board.boardSituation[0][6];
			board.boardSituation[0][6] = NULL;
			board.boardSituation[0][4]->moved--;
			board.boardSituation[0][5]->posX = 7;
			board.boardSituation[0][7] = board.boardSituation[0][5];
			board.boardSituation[0][5] = NULL;
			board.boardSituation[0][7]->moved--;
		}
		else
		{
			board.boardSituation[7][6]->posX = 4;
			board.boardSituation[7][4] = board.boardSituation[7][6];
			board.boardSituation[7][6] = NULL;
			board.boardSituation[7][4]->moved--;
			board.boardSituation[7][5]->posX = 7;
			board.boardSituation[7][7] = board.boardSituation[7][5];
			board.boardSituation[7][5] = NULL;
			board.boardSituation[7][7]->moved--;
		}
	}
	else if (undo_command == "O-O-O")
	{
		if ((stack[stack.size() - 1])[0] == '0')
		{
			board.boardSituation[0][2]->posX = 4;
			board.boardSituation[0][4] = board.boardSituation[0][2];
			board.boardSituation[0][2] = NULL;
			board.boardSituation[0][4]->moved--;
			board.boardSituation[0][3]->posX = 0;
			board.boardSituation[0][0] = board.boardSituation[0][3];
			board.boardSituation[0][3] = NULL;
			board.boardSituation[0][0]->moved--;
		}
		else
		{
			board.boardSituation[7][2]->posX = 4;
			board.boardSituation[7][4] = board.boardSituation[7][2];
			board.boardSituation[7][2] = NULL;
			board.boardSituation[7][4]->moved--;
			board.boardSituation[7][3]->posX = 7;
			board.boardSituation[7][0] = board.boardSituation[7][3];
			board.boardSituation[7][3] = NULL;
			board.boardSituation[7][0]->moved--;
		}
	}
	else
	{
		string undo_command2 = undo_command.substr(8);
		int x1 = undo_command[0] - '0', y1 = undo_command[2] - '0', x2 = undo_command[4] - '0', y2 = undo_command[6] - '0';
		if (board.boardSituation[y1][x1]->type == Pawn && (y2 == 0 || y2 == 7))
		{
			if (undo_command2[0] == 'X')
			{
				board.boardSituation[y2][x2]->posX = x1;
				board.boardSituation[y2][x2]->posY = y1;
				board.boardSituation[y1][x1] = board.boardSituation[y2][x2];
				board.boardSituation[y2][x2] = NULL;
				board.boardSituation[y1][x1]->type = Pawn;
				board.boardSituation[y1][x1]->moved--;
			}
			else if (undo_command2[0] == 'Q')
			{
				board.boardSituation[y2][x2]->posX = x1;
				board.boardSituation[y2][x2]->posY = y1;
				board.boardSituation[y1][x1] = board.boardSituation[y2][x2];
				board.boardSituation[y2][x2] = NULL;
				board.boardSituation[y1][x1]->type = Pawn;
				board.boardSituation[y1][x1]->moved--;
				if ((stack[stack.size() - 1])[0] == '0')
				{
					board.boardSituation[y2][x2] = new ChessPiece(1, Queen, x2, y2);
					players[1]->OwningPiece.push_back(*(board.boardSituation[y2][x2]));
				}
				else
				{
					board.boardSituation[y2][x2] = new ChessPiece(0, Queen, x2, y2);
					players[0]->OwningPiece.push_back(*(board.boardSituation[y2][x2]));
				}
			}
			else if (undo_command2[0] == 'R')
			{
				board.boardSituation[y2][x2]->posX = x1;
				board.boardSituation[y2][x2]->posY = y1;
				board.boardSituation[y1][x1] = board.boardSituation[y2][x2];
				board.boardSituation[y2][x2] = NULL;
				board.boardSituation[y1][x1]->type = Pawn;
				board.boardSituation[y1][x1]->moved--;
				if ((stack[stack.size() - 1])[0] == '0')
				{
					board.boardSituation[y2][x2] = new ChessPiece(1, Rook, x2, y2);
					players[1]->OwningPiece.push_back(*(board.boardSituation[y2][x2]));
				}
				else
				{
					board.boardSituation[y2][x2] = new ChessPiece(0, Rook, x2, y2);
					players[0]->OwningPiece.push_back(*(board.boardSituation[y2][x2]));
				}
			}
			else if (undo_command2[0] == 'B')
			{
				board.boardSituation[y2][x2]->posX = x1;
				board.boardSituation[y2][x2]->posY = y1;
				board.boardSituation[y1][x1] = board.boardSituation[y2][x2];
				board.boardSituation[y2][x2] = NULL;
				board.boardSituation[y1][x1]->type = Pawn;
				board.boardSituation[y1][x1]->moved--;
				if ((stack[stack.size() - 1])[0] == '0')
				{
					board.boardSituation[y2][x2] = new ChessPiece(1, Bishop, x2, y2);
					players[1]->OwningPiece.push_back(*(board.boardSituation[y2][x2]));
				}
				else
				{
					board.boardSituation[y2][x2] = new ChessPiece(0, Bishop, x2, y2);
					players[0]->OwningPiece.push_back(*(board.boardSituation[y2][x2]));
				}
			}
			else if (undo_command2[0] == 'K')
			{
				board.boardSituation[y2][x2]->posX = x1;
				board.boardSituation[y2][x2]->posY = y1;
				board.boardSituation[y1][x1] = board.boardSituation[y2][x2];
				board.boardSituation[y2][x2] = NULL;
				board.boardSituation[y1][x1]->type = Pawn;
				board.boardSituation[y1][x1]->moved--;
				if ((stack[stack.size() - 1])[0] == '0')
				{
					board.boardSituation[y2][x2] = new ChessPiece(1, Knight, x2, y2);
					players[1]->OwningPiece.push_back(*(board.boardSituation[y2][x2]));
				}
				else
				{
					board.boardSituation[y2][x2] = new ChessPiece(0, Knight, x2, y2);
					players[0]->OwningPiece.push_back(*(board.boardSituation[y2][x2]));
				}
			}
		}//升變結束
		else if (undo_command2[0] == 'X')
		{
			//吃過路兵
			string undo_command3 = undo_command2.substr(2);
			if (undo_command3[0] == 'P')
			{
				if ((stack[stack.size() - 1])[0] == '0')
				{
					if (x2 - x1 < 0)
					{
						board.boardSituation[y2][x2]->posX = x1;
						board.boardSituation[y2][x2]->posY = y1;
						board.boardSituation[y1][x1] = board.boardSituation[y2][x2];
						board.boardSituation[y2][x2] = NULL;
						board.boardSituation[y1][x1]->moved--;
						board.boardSituation[y1][x1 - 1] = new ChessPiece(1, Pawn, y1, x1 - 1);
						players[1]->OwningPiece.push_back(*(board.boardSituation[y1][x1 - 1]));
						board.boardSituation[y1][x1 - 1]->moved++;
					}
					else
					{
						board.boardSituation[y2][x2]->posX = x1;
						board.boardSituation[y2][x2]->posY = y1;
						board.boardSituation[y1][x1] = board.boardSituation[y2][x2];
						board.boardSituation[y2][x2] = NULL;
						board.boardSituation[y1][x1]->moved--;
						board.boardSituation[y1][x1 + 1] = new ChessPiece(1, Pawn, y1, x1 + 1);
						players[1]->OwningPiece.push_back(*(board.boardSituation[y1][x1 + 1]));
						board.boardSituation[y1][x1 - 1]->moved++;
					}
				}
				else
				{
					if (x2 - x1 < 0)
					{
						board.boardSituation[y2][x2]->posX = x1;
						board.boardSituation[y2][x2]->posY = y1;
						board.boardSituation[y1][x1] = board.boardSituation[y2][x2];
						board.boardSituation[y2][x2] = NULL;
						board.boardSituation[y1][x1]->moved--;
						board.boardSituation[y1][x1 - 1] = new ChessPiece(0, Pawn, y1, x1 - 1);
						players[1]->OwningPiece.push_back(*(board.boardSituation[y1][x1 - 1]));
						board.boardSituation[y1][x1 - 1]->moved++;
					}
					else
					{
						board.boardSituation[y2][x2]->posX = x1;
						board.boardSituation[y2][x2]->posY = y1;
						board.boardSituation[y1][x1] = board.boardSituation[y2][x2];
						board.boardSituation[y2][x2] = NULL;
						board.boardSituation[y1][x1]->moved--;
						board.boardSituation[y1][x1 + 1] = new ChessPiece(0, Pawn, y1, x1 + 1);
						players[1]->OwningPiece.push_back(*(board.boardSituation[y1][x1 + 1]));
						board.boardSituation[y1][x1 - 1]->moved++;
					}
				}
			}
			else
			{//普通走法
				board.boardSituation[y2][x2]->posX = x1;
				board.boardSituation[y2][x2]->posY = y1;
				board.boardSituation[y1][x1] = board.boardSituation[y2][x2];
				board.boardSituation[y2][x2] = NULL;
			}
		}
		else if (undo_command2[0] == 'Q')
		{
			board.boardSituation[y2][x2]->posX = x1;
			board.boardSituation[y2][x2]->posY = y1;
			board.boardSituation[y1][x1] = board.boardSituation[y2][x2];
			board.boardSituation[y2][x2] = NULL;
			if ((stack[stack.size() - 1])[0] == '0')
			{
				board.boardSituation[y2][x2] = new ChessPiece(1, Queen, x2, y2);
				players[1]->OwningPiece.push_back(*(board.boardSituation[y2][x2]));
				board.boardSituation[y2][x2]->moved++;
			}
			else
			{
				board.boardSituation[y2][x2] = new ChessPiece(0, Queen, x2, y2);
				players[0]->OwningPiece.push_back(*(board.boardSituation[y2][x2]));
				board.boardSituation[y2][x2]->moved++;
			}
		}
		else if (undo_command2[0] == 'R')
		{
			board.boardSituation[y2][x2]->posX = x1;
			board.boardSituation[y2][x2]->posY = y1;
			board.boardSituation[y1][x1] = board.boardSituation[y2][x2];
			board.boardSituation[y2][x2] = NULL;
			if ((stack[stack.size() - 1])[0] == '0')
			{
				board.boardSituation[y2][x2] = new ChessPiece(1, Rook, x2, y2);
				players[1]->OwningPiece.push_back(*(board.boardSituation[y2][x2]));
				board.boardSituation[y2][x2]->moved++;
			}
			else
			{
				board.boardSituation[y2][x2] = new ChessPiece(0, Rook, x2, y2);
				players[0]->OwningPiece.push_back(*(board.boardSituation[y2][x2]));
				board.boardSituation[y2][x2]->moved++;
			}
		}
		else if (undo_command2[0] == 'B')
		{
			board.boardSituation[y2][x2]->posY = y1;
			board.boardSituation[y1][x1] = board.boardSituation[y2][x2];
			board.boardSituation[y2][x2] = NULL;
			if ((stack[stack.size() - 1])[0] == '0')
			{
				board.boardSituation[y2][x2] = new ChessPiece(1, Bishop, x2, y2);
				players[1]->OwningPiece.push_back(*(board.boardSituation[y2][x2]));
				board.boardSituation[y2][x2]->moved++;
			}
			else
			{
				board.boardSituation[y2][x2] = new ChessPiece(0, Bishop, x2, y2);
				players[0]->OwningPiece.push_back(*(board.boardSituation[y2][x2]));
				board.boardSituation[y2][x2]->moved++;
			}
		}
		else if (undo_command2[0] == 'K')
		{
			board.boardSituation[y2][x2]->posY = y1;
			board.boardSituation[y1][x1] = board.boardSituation[y2][x2];
			board.boardSituation[y2][x2] = NULL;
			if ((stack[stack.size() - 1])[0] == '0')
			{
				board.boardSituation[y2][x2] = new ChessPiece(1, Knight, x2, y2);
				players[1]->OwningPiece.push_back(*(board.boardSituation[y2][x2]));
				board.boardSituation[y2][x2]->moved++;
			}
			else
			{
				board.boardSituation[y2][x2] = new ChessPiece(0, Knight, x2, y2);
				players[0]->OwningPiece.push_back(*(board.boardSituation[y2][x2]));
				board.boardSituation[y2][x2]->moved++;
			}
		}
		else if (undo_command2[0] == 'P')
		{
			board.boardSituation[y2][x2]->posX = x1;
			board.boardSituation[y2][x2]->posY = y1;
			board.boardSituation[y1][x1] = board.boardSituation[y2][x2];
			board.boardSituation[y2][x2] = NULL;
			if ((stack[stack.size() - 1])[0] == '0')
			{
				board.boardSituation[y2][x2] = new ChessPiece(1, Pawn, x2, y2);
				players[1]->OwningPiece.push_back(*(board.boardSituation[y2][x2]));
				board.boardSituation[y2][x2]->moved++;
			}
			else
			{
				board.boardSituation[y2][x2] = new ChessPiece(0, Pawn, x2, y2);
				players[0]->OwningPiece.push_back(*(board.boardSituation[y2][x2]));
				board.boardSituation[y2][x2]->moved++;
			}
		}
	}
}

void redo(vector<string>& stack, vector<string>& l_log, Board& board, Player** players)
{
	if (stack.size() < 1) { return; }
	string undo_command = stack[stack.size() - 1].substr(2);
	if (undo_command == "O-O")
	{
		if ((stack[stack.size() - 1])[0] == '0')
		{
			board.boardSituation[0][4]->posX = 6;
			board.boardSituation[0][6] = board.boardSituation[0][4];
			board.boardSituation[0][4] = NULL;
			board.boardSituation[0][6]->moved++;
			board.boardSituation[0][7]->posX = 5;
			board.boardSituation[0][5] = board.boardSituation[0][7];
			board.boardSituation[0][7] = NULL;
			board.boardSituation[0][5]->moved++;
		}
		else
		{
			board.boardSituation[7][4]->posX = 6;
			board.boardSituation[7][6] = board.boardSituation[7][4];
			board.boardSituation[7][4] = NULL;
			board.boardSituation[7][6]->moved++;
			board.boardSituation[7][7]->posX = 5;
			board.boardSituation[7][5] = board.boardSituation[7][7];
			board.boardSituation[7][7] = NULL;
			board.boardSituation[7][5]->moved++;
		}
		l_log.push_back(stack[stack.size() - 1]);
		stack.pop_back();
	}
	else if (undo_command == "O-O-O")
	{
		if ((stack[stack.size() - 1])[0] == '0')
		{
			board.boardSituation[0][4]->posX = 2;
			board.boardSituation[0][2] = board.boardSituation[0][4];
			board.boardSituation[0][4] = NULL;
			board.boardSituation[0][2]->moved++;
			board.boardSituation[0][0]->posX = 3;
			board.boardSituation[0][3] = board.boardSituation[0][0];
			board.boardSituation[0][0] = NULL;
			board.boardSituation[0][3]->moved++;

		}
		else
		{
			board.boardSituation[7][4]->posX = 6;
			board.boardSituation[7][2] = board.boardSituation[7][4];
			board.boardSituation[7][4] = NULL;
			board.boardSituation[7][2]->moved++;
			board.boardSituation[7][0]->posX = 5;
			board.boardSituation[7][3] = board.boardSituation[7][0];
			board.boardSituation[7][0] = NULL;
			board.boardSituation[7][3]->moved++;
		}
		l_log.push_back(stack[stack.size() - 1]);
		stack.pop_back();
	}
	else
	{
		int x1 = undo_command[0] - '0', y1 = undo_command[2] - '0', x2 = undo_command[4] - '0', y2 = undo_command[6] - '0';
		string undo_command2 = undo_command.substr(2);
		if (y2 == 0 || y2 == 7)
		{//升變
			if (undo_command2[0] == 'X')
			{
				board.boardSituation[y2][x2] = board.boardSituation[y1][x1];
				board.boardSituation[y1][x1] = NULL;
				board.boardSituation[y2][x2]->moved++;
				board.boardSituation[y2][x2]->posX = x2;
				board.boardSituation[y2][x2]->posY = y2;
				if (undo_command2[2] == 'Q')
				{
					board.boardSituation[y2][x2]->type = Queen;
				}
				else if (undo_command2[2] == 'B')
				{
					board.boardSituation[y2][x2]->type = Bishop;
				}
				else if (undo_command2[2] == 'K')
				{
					board.boardSituation[y2][x2]->type = Knight;
				}
				else if (undo_command2[2] == 'R')
				{
					board.boardSituation[y2][x2]->type = Rook;
				}
				l_log.push_back(stack[stack.size() - 1]);
				stack.pop_back();
			}
			else
			{
				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < players[i]->OwningPiece.size(); i++)
					{
						if (players[i]->OwningPiece[j].posX == x2 && players[i]->OwningPiece[j].posY == y2)
						{
							players[i]->OwningPiece.erase(players[i]->OwningPiece.begin() + j);
						}
					}
				}
				board.boardSituation[y2][x2] = board.boardSituation[y1][x1];
				board.boardSituation[y1][x1] = NULL;
				board.boardSituation[y2][x2]->moved++;
				board.boardSituation[y2][x2]->posX = x2;
				board.boardSituation[y2][x2]->posY = y2;
				if (undo_command2[2] == 'Q')
				{
					board.boardSituation[y2][x2]->type = Queen;
				}
				else if (undo_command2[2] == 'B')
				{
					board.boardSituation[y2][x2]->type = Bishop;
				}
				else if (undo_command2[2] == 'K')
				{
					board.boardSituation[y2][x2]->type = Knight;
				}
				else if (undo_command2[2] == 'R')
				{
					board.boardSituation[y2][x2]->type = Rook;
				}
				l_log.push_back(stack[stack.size() - 1]);
				stack.pop_back();
			}
		}
		else if (board.boardSituation[y2][x2] == NULL)
		{
			if (undo_command2[0] == 'X' && undo_command2[2] == 'P')
			{
				//吃過路兵
				board.boardSituation[y1][x1]->posX = x2;
				board.boardSituation[y1][x1]->posY = y2;
				board.boardSituation[y2][x2] = board.boardSituation[y1][x1];
				board.boardSituation[y1][x1] = NULL;
				if (x2 - x1 < 0)
				{
					for (int i = 0; i < 2; i++)
					{
						for (int j = 0; j < players[i]->OwningPiece.size(); i++)
						{
							if (players[i]->OwningPiece[j].posX == x1 - 1 && players[i]->OwningPiece[j].posY == y1)
							{
								players[i]->OwningPiece.erase(players[i]->OwningPiece.begin() + j);
							}
						}
					}
					board.boardSituation[y1][x1 - 1] = NULL;
				}
				else
				{
					for (int i = 0; i < 2; i++)
					{
						for (int j = 0; j < players[i]->OwningPiece.size(); i++)
						{
							if (players[i]->OwningPiece[j].posX == x1 + 1 && players[i]->OwningPiece[j].posY == y1)
							{
								players[i]->OwningPiece.erase(players[i]->OwningPiece.begin() + j);
							}
						}
					}
					board.boardSituation[y1][x1 + 1] = NULL;
				}
			}
			else
			{
				board.boardSituation[y2][x2] = board.boardSituation[y1][x1];
				board.boardSituation[y1][x1] = NULL;
				board.boardSituation[y2][x2]->moved++;
				l_log.push_back(stack[stack.size() - 1]);
				stack.pop_back();
			}
			l_log.push_back(stack[stack.size() - 1]);
			stack.pop_back();
		}
		else
		{
			if ((stack[stack.size() - 1])[0] == 0)
			{
				for (int i = 0; i < players[0]->OwningPiece.size(); i++)
				{
					if (players[0]->OwningPiece[i].posX == x2 && players[0]->OwningPiece[i].posY == y2)
					{
						players[0]->OwningPiece.erase(players[0]->OwningPiece.begin() + i);
					}
				}
				board.boardSituation[y2][x2] = board.boardSituation[y1][x1];
				board.boardSituation[y1][x1] = NULL;
				board.boardSituation[y2][x2]->moved++;
				l_log.push_back(stack[stack.size() - 1]);
				stack.pop_back();
			}
			else
			{
				for (int i = 0; i < players[1]->OwningPiece.size(); i++)
				{
					if (players[1]->OwningPiece[i].posX == x2 && players[1]->OwningPiece[i].posY == y2)
					{
						players[1]->OwningPiece.erase(players[1]->OwningPiece.begin() + i);
					}
				}
				board.boardSituation[y2][x2] = board.boardSituation[y1][x1];
				board.boardSituation[y1][x1] = NULL;
				board.boardSituation[y2][x2]->moved++;
				l_log.push_back(stack[stack.size() - 1]);
				stack.pop_back();
			}
		}
	}
}
