#include "Viewer.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
using namespace std;
using namespace cv;
#define SIZE 50

Viewer::Viewer() {
	Screen = Mat(SIZE * 10, SIZE * 10, CV_8UC3, Scalar(200, 200, 200));
}

void Viewer::drawMenu() {
	string text = "Chess Game";
	int thickness = 2;
	double font_scale = 2;
	int baseline;
	Size text_size = getTextSize(text, 0, font_scale, thickness, &baseline);
	int x = Screen.cols / 2 - text_size.width / 2;
	int y = Screen.rows / 2 + text_size.height / 2;
	putText(Screen, text, Point(x, y), 0, font_scale, Scalar(0, 0, 0), thickness);
	rectangle(Screen, Point(SIZE * 1, SIZE * 7), Point(SIZE * 4.5, SIZE * 8), Scalar(0, 0, 0), 2);
	text = "New Game";
	font_scale = 1;
	thickness = 1;
	text_size = getTextSize(text, 0, font_scale, thickness, &baseline);
	x = SIZE * 2.75 - text_size.width / 2;
	y = SIZE * 7.5 + text_size.height / 2;
	putText(Screen, text, Point(x, y), 0, font_scale, Scalar(0, 0, 0), thickness);
	rectangle(Screen, Point(SIZE * 5.5, SIZE * 7), Point(SIZE * 9, SIZE * 8), Scalar(0, 0, 0), 2);
	text = "Exit";
	text_size = getTextSize(text, 0, font_scale, thickness, &baseline);
	x = SIZE * 7.25 - text_size.width / 2;
	y = SIZE * 7.5 + text_size.height / 2;
	putText(Screen, text, Point(x, y), 0, font_scale, Scalar(0, 0, 0), thickness);
}

Point Viewer::BoradtoImg(int x, int y) {
	Point A;
	if (Viewer::plateFace == 0) {
		A.x = SIZE * (x + 1);
		A.y = SIZE * (8 - y);
	}
	else {
		A.x = SIZE * (8 - x);
		A.y = SIZE * (y + 1);
	}
	return A;
}

bool Viewer::mergeImg(Mat& dst, Mat& src, Point location) {
	double scale = 1.0, size = 1.0, angle = 0;
	if (dst.channels() != 3 || src.channels() != 4 || location.x > dst.cols || location.y > dst.cols) {
		return false;
	}
	Mat small_size = src.clone();

	if (size != 1 || angle != 0) {
		int width = src.cols > (dst.cols - location.x) ? (dst.cols - location.x) : src.cols;
		int length = src.rows > (dst.rows - location.y) ? (dst.rows - location.y) : src.rows;
		Mat rotation = getRotationMatrix2D(Point2f(length / 2, width / 2), angle, size);
		warpAffine(small_size, small_size, rotation, Size(width, length));
	}

	Mat dst_part(dst, Rect(location.x, location.y, small_size.cols, small_size.rows));

	vector<Mat>src_channels;
	vector<Mat>dst_channels;
	split(small_size, src_channels);
	split(dst_part, dst_channels);

	if (scale < 1) {
		src_channels[3] *= scale;
		scale = 1;
	}
	for (int i = 0; i < 3; i++) {
		dst_channels[i] = dst_channels[i].mul(255.0 / scale - src_channels[3], scale / 255.0);
		dst_channels[i] += src_channels[i].mul(src_channels[3], scale / 255.0);
	}
	merge(dst_channels, dst_part);
	return true;
}

void Viewer::drawBoard() {
	rectangle(Screen, Point(0, 0), Point(SIZE * 10, SIZE * 10), Scalar(200, 200, 200), -1);
	int thickness = 1;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((i + j) % 2 == 0) {
				rectangle(Screen, Point(SIZE * (i + 1), SIZE * (j + 1)), Point(SIZE * (i + 2), SIZE * (j + 2)), Scalar(158, 206, 255), -1);
			}
			else {
				rectangle(Screen, Point(SIZE * (i + 1), SIZE * (j + 1)), Point(SIZE * (i + 2), SIZE * (j + 2)), Scalar(71, 131, 209), -1);
			}
			rectangle(Screen, Point(SIZE * (i + 1), SIZE * (j + 1)), Point(SIZE * (i + 2), SIZE * (j + 2)), Scalar(0, 0, 0), thickness);
		}
	}
	string text = "Chess Game";
	double font_scale = 1;
	int baseline;
	Size text_size = getTextSize(text, 0, font_scale, thickness, &baseline);
	int x = Screen.cols / 2 - text_size.width / 2;
	int y = SIZE / 2 + text_size.height / 2;
	putText(Screen, text, Point(x, y), 0, font_scale, Scalar(0, 0, 0), thickness);

	string letter = "0";
	if (Viewer::plateFace == 0) {
		for (int i = 0; i < 8; i++) {
			letter[0] = i + 'a';
			text_size = getTextSize(letter, 0, font_scale, thickness, &baseline);
			int x = SIZE * (i + 1.5) - text_size.width / 2;
			int y = SIZE * 9.75;
			putText(Screen, letter, Point(x, y), 0, font_scale, Scalar(0, 0, 0), thickness);
		}
		for (int i = 8; i > 0; i--) {
			letter[0] = i + '0';
			text_size = getTextSize(letter, 0, font_scale, thickness, &baseline);
			int x = SIZE / 4;
			int y = SIZE * (9.5 - i) + text_size.height / 2;
			putText(Screen, letter, Point(x, y), 0, font_scale, Scalar(0, 0, 0), thickness);
		}
	}
	else {
		for (int i = 7; i >= 0; i--) {
			letter[0] = 'h' - i;
			text_size = getTextSize(letter, 0, font_scale, thickness, &baseline);
			int x = SIZE * (i + 1.5) - text_size.width / 2;
			int y = SIZE * 9.75;
			putText(Screen, letter, Point(x, y), 0, font_scale, Scalar(0, 0, 0), thickness);
		}
		for (int i = 1; i <= 8; i++) {
			letter[0] = '9' - i;
			text_size = getTextSize(letter, 0, font_scale, thickness, &baseline);
			int x = SIZE / 4;
			int y = SIZE * (9.5 - i) + text_size.height / 2;
			putText(Screen, letter, Point(x, y), 0, font_scale, Scalar(0, 0, 0), thickness);
		}
	}
}

void Viewer::drawButton(int undo, int redo, int FF) {
	string text = "REV.";
	double font_scale = 0.5;
	int thickness = 1;
	int baseline;
	Size text_size = getTextSize(text, 0, font_scale, thickness, &baseline);
	int x = SIZE * 9.5 - text_size.width / 2;
	int y = SIZE * 1.125 + text_size.height;
	putText(Screen, text, Point(x, y), 0, font_scale, Scalar(0, 0, 0), thickness);
	rectangle(Screen, Point(SIZE * 9.125, SIZE * 1.5), Point(SIZE * 9.875, SIZE * 2), Scalar(127, 127, 127), -1);
	rectangle(Screen, Point(SIZE * 9.125, SIZE * 1.5), Point(SIZE * 9.875, SIZE * 2), Scalar(0, 0, 0), thickness);

	text = "undo";

	text_size = getTextSize(text, 0, font_scale, thickness, &baseline);
	x = SIZE * 9.5 - text_size.width / 2;
	y = SIZE * 2.125 + text_size.height;
	putText(Screen, text, Point(x, y), 0, font_scale, Scalar(0, 0, 0), thickness);
	if (undo == 1) {
		rectangle(Screen, Point(SIZE * 9.125, SIZE * 2.5), Point(SIZE * 9.875, SIZE * 3), Scalar(82, 173, 97), -1);
	}
	rectangle(Screen, Point(SIZE * 9.125, SIZE * 2.5), Point(SIZE * 9.875, SIZE * 3), Scalar(0, 0, 0), thickness);

	text = "redo";
	text_size = getTextSize(text, 0, font_scale, thickness, &baseline);
	x = SIZE * 9.5 - text_size.width / 2;
	y = SIZE * 3.125 + text_size.height;
	putText(Screen, text, Point(x, y), 0, font_scale, Scalar(0, 0, 0), thickness);
	if (redo == 1) {
		rectangle(Screen, Point(SIZE * 9.125, SIZE * 3.5), Point(SIZE * 9.875, SIZE * 4), Scalar(82, 173, 97), -1);
	}
	rectangle(Screen, Point(SIZE * 9.125, SIZE * 3.5), Point(SIZE * 9.875, SIZE * 4), Scalar(0, 0, 0), thickness);

	text = "FF.";
	text_size = getTextSize(text, 0, font_scale, thickness, &baseline);
	x = SIZE * 9.5 - text_size.width / 2;
	y = SIZE * 6.125 + text_size.height;
	putText(Screen, text, Point(x, y), 0, font_scale, Scalar(0, 0, 0), thickness);
	if (FF == 1) {
		rectangle(Screen, Point(SIZE * 9.125, SIZE * 6.5), Point(SIZE * 9.875, SIZE * 7), Scalar(31, 23, 176), -1);
	}
	else {
		rectangle(Screen, Point(SIZE * 9.125, SIZE * 6.5), Point(SIZE * 9.875, SIZE * 7), Scalar(127, 127, 127), -1);
	}
	rectangle(Screen, Point(SIZE * 9.125, SIZE * 6.5), Point(SIZE * 9.875, SIZE * 7), Scalar(0, 0, 0), thickness);


}

void Viewer::drawTurn(int player) {
	if (player == 0) {
		rectangle(Screen, Point(SIZE * 9.125, SIZE * 4.625), Point(SIZE * 9.875, SIZE * 5.375), Scalar(255, 255, 255), -1);
	}
	else {
		rectangle(Screen, Point(SIZE * 9.125, SIZE * 4.625), Point(SIZE * 9.875, SIZE * 5.375), Scalar(0, 0, 0), -1);
	}
	rectangle(Screen, Point(SIZE * 9.125, SIZE * 4.625), Point(SIZE * 9.875, SIZE * 5.375), Scalar(127, 127, 127), 1);
}

void Viewer::drawChess(ChessPiece piece) {
	Mat chessImg;
	if (piece.player == 0) {
		switch (piece.type) {
		case King:
			chessImg = imread("Wking.png", -1);
			break;
		case Queen:
			chessImg = imread("Wqueen.png", -1);
			break;
		case Rook:
			chessImg = imread("Wrook.png", -1);
			break;
		case Bishop:
			chessImg = imread("Wbishop.png", -1);
			break;
		case Knight:
			chessImg = imread("Wknight.png", -1);
			break;
		case Pawn:
			chessImg = imread("Wpawn.png", -1);
			break;
		default:
			break;
		}
	}
	if (piece.player == 1) {
		switch (piece.type) {
		case King:
			chessImg = imread("Bking.png", -1);
			break;
		case Queen:
			chessImg = imread("Bqueen.png", -1);
			break;
		case Rook:
			chessImg = imread("Brook.png", -1);
			break;
		case Bishop:
			chessImg = imread("Bbishop.png", -1);
			break;
		case Knight:
			chessImg = imread("Bknight.png", -1);
			break;
		case Pawn:
			chessImg = imread("Bpawn.png", -1);
			break;
		default:
			break;
		}
	}
	Point A = BoradtoImg(piece.posX, piece.posY);
	if (Viewer::plateFace == 0) {
		A.x += (SIZE - chessImg.cols) / 2;
		A.y += (SIZE - chessImg.rows) / 2;

	}
	else {
		A.x += (SIZE - chessImg.cols) / 2;
		A.y += (SIZE - chessImg.rows) / 2;
	}
	mergeImg(Screen, chessImg, A);
}

void Viewer::drawMovable(Board board, int x, int y) {
	board.checkMovable(*board.boardSituation[x][y]);
	for (int i = 0; i < board.boardSituation[x][y]->movableX.size(); i++) {
		Point A = BoradtoImg(board.boardSituation[x][y]->movableX[i], board.boardSituation[x][y]->movableY[i]);
		rectangle(Screen, A, Point(A.x + SIZE, A.y + SIZE), Scalar(82, 173, 97), -1);
		rectangle(Screen, A, Point(A.x + SIZE, A.y + SIZE), Scalar(0, 0, 0), 1);
	}
	for (int i = 0; i < board.boardSituation[x][y]->capturableX.size(); i++) {
		Point A = BoradtoImg(board.boardSituation[x][y]->capturableX[i], board.boardSituation[x][y]->capturableY[i]);
		rectangle(Screen, A, Point(A.x + SIZE, A.y + SIZE), Scalar(31, 23, 176), -1);
		rectangle(Screen, A, Point(A.x + SIZE, A.y + SIZE), Scalar(0, 0, 0), 1);
		drawChess(*board.boardSituation[board.boardSituation[x][y]->capturableX[i]][board.boardSituation[x][y]->capturableY[i]]);
	}
	Point B = BoradtoImg(board.boardSituation[x][y]->posX, board.boardSituation[x][y]->posY);
	rectangle(Screen, B, Point(B.x + SIZE, B.y + SIZE), Scalar(31, 23, 230), 2);
}

void Viewer::drawPromotingTips(int player) {
	Scalar color;
	if (player == 1) {
		color = Scalar(0, 0, 0);
	}
	else {
		color = Scalar(255, 255, 255);
	}
	rectangle(Screen, Point(SIZE * 2.125, SIZE * 3.625), Point(SIZE * 7.875, SIZE * 6.375), Scalar(200, 200, 200), -1);
	rectangle(Screen, Point(SIZE * 2.125, SIZE * 3.625), Point(SIZE * 7.875, SIZE * 6.375), color, 1);
	string text = "Promotion";
	double font_scale = 1;
	int baseline;
	int thickness = 1;
	Size text_size = getTextSize(text, 0, font_scale, thickness, &baseline);
	int x = Screen.cols / 2 - text_size.width / 2;
	int y = SIZE * 4.5 + text_size.height / 2;
	putText(Screen, text, Point(x, y), 0, font_scale, color, thickness);
	for (int i = 0; i < 4; i++) {
		rectangle(Screen, Point(SIZE * (2.8125 + i * 1.125), SIZE * 5), Point(SIZE * (3.8125 + i * 1.125), SIZE * 6), color, thickness);
		Mat chessImg;
		switch (i) {
		case 0:
			if (player == 0) {
				chessImg = imread("Wqueen.png", -1);
			}
			else {
				chessImg = imread("Bqueen.png", -1);
			}
			break;
		case 1:
			if (player == 0) {
				chessImg = imread("Wrook.png", -1);
			}
			else {
				chessImg = imread("Brook.png", -1);
			}
			break;
		case 2:
			if (player == 0) {
				chessImg = imread("Wbishop.png", -1);
			}
			else {
				chessImg = imread("Bbishop.png", -1);
			}
			break;
		case 3:
			if (player == 0) {
				chessImg = imread("Wknight.png", -1);
			}
			else {
				chessImg = imread("Bknight.png", -1);
			}
			break;
		default:
			break;
		}
		Point A(SIZE * (2.8125 + i * 1.125), SIZE * 5);
		A.x += (SIZE - chessImg.cols) / 2;
		A.y += (SIZE - chessImg.rows) / 2;
		mergeImg(Screen, chessImg, A);
	}
}

void Viewer::drawCheck(ChessPiece piece) {
	Point A = BoradtoImg(piece.posX, piece.posY);
	rectangle(Screen, A, Point(A.x + SIZE, A.y + SIZE), Scalar(31, 23, 176), -1);
	rectangle(Screen, A, Point(A.x + SIZE, A.y + SIZE), Scalar(0, 0, 0), 1);
	drawChess(piece);
}

void Viewer::drawCheckmate(int loser) {
	Scalar winner;
	if (loser == 0) {
		winner = Scalar(0, 0, 0);
	}
	else {
		winner = Scalar(255, 255, 255);
	}
	rectangle(Screen, Point(SIZE * 2, SIZE * 4), Point(SIZE * 8, SIZE * 6), Scalar(200, 200, 200), -1);
	rectangle(Screen, Point(SIZE * 2, SIZE * 4), Point(SIZE * 8, SIZE * 6), winner, 1);
	string text = "CHECKMATE!";
	double font_scale = 1.5;
	int baseline;
	int thickness = 2;
	Size text_size = getTextSize(text, 0, font_scale, thickness, &baseline);
	int x = Screen.cols / 2 - text_size.width / 2;
	int y = SIZE * 4.5 + text_size.height / 2;
	putText(Screen, text, Point(x, y), 0, font_scale, winner, thickness);
	if (loser == 0) {
		text = "Black won.";
	}
	else {
		text = "White won.";
	}
	font_scale = 1;
	thickness = 1;
	text_size = getTextSize(text, 0, font_scale, thickness, &baseline);
	x = Screen.cols / 2 - SIZE - text_size.width / 2;
	y = SIZE * 5.5 + text_size.height / 2;
	putText(Screen, text, Point(x, y), 0, font_scale, winner, thickness);
	rectangle(Screen, Point(SIZE * 6.25, SIZE * 5.25), Point(SIZE * 7.75, SIZE * 5.75), winner, 1);
	text = "menu";
	font_scale = 0.5;
	text_size = getTextSize(text, 0, font_scale, thickness, &baseline);
	x = SIZE * 7 - text_size.width / 2;
	y = SIZE * 5.5 + text_size.height / 2;
	putText(Screen, text, Point(x, y), 0, font_scale, winner, thickness);
}

void Viewer::drawStalemate(int loser) {
	Scalar winner;
	if (loser == 1) {
		winner = Scalar(0, 0, 0);
	}
	else if (loser == 0) {
		winner = Scalar(255, 255, 255);
	}
	else {
		winner = Scalar(0, 0, 0);
	}
	rectangle(Screen, Point(SIZE * 2, SIZE * 4), Point(SIZE * 8, SIZE * 6), Scalar(200, 200, 200), -1);
	rectangle(Screen, Point(SIZE * 2, SIZE * 4), Point(SIZE * 8, SIZE * 6), winner, 1);
	string text = "Stalemate";
	double font_scale = 1.5;
	int baseline;
	int thickness = 2;
	Size text_size = getTextSize(text, 0, font_scale, thickness, &baseline);
	int x = Screen.cols / 2 - text_size.width / 2;
	int y = SIZE * 4.5 + text_size.height / 2;
	putText(Screen, text, Point(x, y), 0, font_scale, winner, thickness);
	if (loser == 1) {
		text = "Black can't move.";
	}
	else if (loser == 0) {
		text = "White can't move.";
	}
	else {
		text = "Time's up.";
	}
	font_scale = 0.5;
	thickness = 1;
	text_size = getTextSize(text, 0, font_scale, thickness, &baseline);
	x = Screen.cols / 2 - SIZE - text_size.width / 2;
	y = SIZE * 5.5 + text_size.height / 2;
	putText(Screen, text, Point(x, y), 0, font_scale, winner, thickness);
	rectangle(Screen, Point(SIZE * 6.25, SIZE * 5.25), Point(SIZE * 7.75, SIZE * 5.75), winner, 1);
	text = "menu";
	text_size = getTextSize(text, 0, font_scale, thickness, &baseline);
	x = SIZE * 7 - text_size.width / 2;
	y = SIZE * 5.5 + text_size.height / 2;
	putText(Screen, text, Point(x, y), 0, font_scale, winner, thickness);
}
