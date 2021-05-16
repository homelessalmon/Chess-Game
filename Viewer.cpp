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

Point Viewer::BoradtoImg(int x, int y) {
	Point A;
	A.x = SIZE * (x + 1);
	A.y = SIZE * (8 - y);
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

void Viewer::drawButton(int undo, int redo) {
	string text = "undo";
	double font_scale = 0.5;
	int thickness = 1;
	int baseline;
	Size text_size = getTextSize(text, 0, font_scale, thickness, &baseline);
	int x = SIZE * 9.5 - text_size.width / 2;
	int y = SIZE * 2.125 + text_size.height;
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
	if (undo == 1) {
		rectangle(Screen, Point(SIZE * 9.125, SIZE * 3.5), Point(SIZE * 9.875, SIZE * 4), Scalar(82, 173, 97), -1);
	}
	rectangle(Screen, Point(SIZE * 9.125, SIZE * 3.5), Point(SIZE * 9.875, SIZE * 4), Scalar(0, 0, 0), thickness);

	text = "FF.";
	text_size = getTextSize(text, 0, font_scale, thickness, &baseline);
	x = SIZE * 9.5 - text_size.width / 2;
	y = SIZE * 6.125 + text_size.height;
	putText(Screen, text, Point(x, y), 0, font_scale, Scalar(0, 0, 0), thickness);
	rectangle(Screen, Point(SIZE * 9.125, SIZE * 6.5), Point(SIZE * 9.875, SIZE * 7), Scalar(31, 23, 176), -1);
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
	A.x += (SIZE - chessImg.cols) / 2;
	A.y += (SIZE - chessImg.rows) / 2;
	mergeImg(Screen, chessImg, A);
}

//void Viewer::drawMovable(ChessPiece piece) {
//	
//	for (int i = 0; i < piece.movableX.size(); i++) {
//		Point A = BoradtoImg(piece.movableX[i], piece.movableY[i]);
//		rectangle(Screen, A, Point(A.x + SIZE, A.y + SIZE), Scalar(82, 173, 97), -1);
//	}
//}

void Viewer::drawMovable(Board board, int x, int y) {
	board.checkMovable(*board.boardSituation[x][y]);
	for (int i = 0; i < board.boardSituation[x][y]->movableX.size(); i++) {
		Point A = BoradtoImg(board.boardSituation[x][y]->movableX[i], board.boardSituation[x][y]->movableY[i]);
		rectangle(Screen, A, Point(A.x + SIZE, A.y + SIZE), Scalar(82, 173, 97), -1);
	}
	for (int i = 0; i < board.boardSituation[x][y]->capturableX.size(); i++) {
		Point A = BoradtoImg(board.boardSituation[x][y]->capturableX[i], board.boardSituation[x][y]->capturableY[i]);
		rectangle(Screen, A, Point(A.x + SIZE, A.y + SIZE), Scalar(31, 23, 176), -1);
		drawChess(*board.boardSituation[board.boardSituation[x][y]->capturableX[i]][board.boardSituation[x][y]->capturableY[i]]);
	}
}

void Viewer::drawPromotingTips(int player) {

}
