#pragma once
#include <opencv2/opencv.hpp>
#include "ChessPiece.h"
#include "Board.h"
using namespace std;
using namespace cv;
typedef class ChessPiece ChessPiece;
#define SIZE 50

class Viewer {
public:
	static int plateFace;
	Viewer();
	void drawMenu();
	void drawBoard(); //繪製棋盤、底板
	void drawButton(int replay, int FF);
	void drawTurn(int player);
	void drawChess(ChessPiece piece); //繪製piece對應的棋子
	void drawMovable(Board board, int x, int y); //繪製piece可走的方格
	void drawPromotingTips(int player);
	void drawCheck(ChessPiece piece);
	void drawCheckmateAnimation(double i, int loser);
	void drawStalemateAnimation(double i, int loser);
	void drawCheckmate(int loser);
	void drawStalemate(int loser);
	Mat Screen;
private:
	Point BoradtoImg(int x, int y); //得出某棋盤格左上角的座標
	bool mergeImg(Mat& dst, Mat& src, Point location);
};
