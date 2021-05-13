#pragma once
#include <opencv2/opencv.hpp>
#include "ChessPiece.h"
using namespace std;
using namespace cv;
typedef class ChessPiece ChessPiece;
#define SIZE 50

class Viewer {
public:
	Viewer();
	void drawBoard(); //繪製棋盤、底板
	void drawChess(ChessPiece piece); //繪製piece對應的棋子
	void drawMovable(ChessPiece piece); //繪製piece可走的方格
	Mat Screen;
	static void mouseCallback(int event, int x, int y, int flags, void* param);
	void doMouseCallback(int event, int x, int y, int flags);
private:
	Point BoradtoImg(int x, int y); //得出某棋盤格左上角的座標
	int ImgtoBoradX(Point P); //得出P點對應的棋盤X座標
	int ImgtoBoradY(Point P); //得出P點對應的棋盤Y座標
	bool mergeImg(Mat& dst, Mat& src, Point location);
};
