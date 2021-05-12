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
	void drawBoard();
	void drawChess(ChessPiece piece);
	void drawMovable(ChessPiece piece);
	Mat Screen;
	static void mouseCallback(int event, int x, int y, int flags, void *param);
	void doMouseCallback(int event, int x, int y, int flags);
private:
	Point BoradtoImg(int x, int y);
	int ImgtoBoradX(Point P);
	int ImgtoBoradY(Point P);
	bool mergeImg(Mat& dst, Mat& src, Point location);
};
