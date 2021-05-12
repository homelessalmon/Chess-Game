#pragma once
#include <opencv2/opencv.hpp>
#include "ChessPiece.h"
#define SIZE 50
using namespace std;
using namespace cv;
typedef class ChessPiece ChessPiece;

class Viewer {
public:
	Viewer();
	void drawBoard();
	void drawChess(ChessPiece piece);
	void drawMovable(ChessPiece piece);
	Mat Screen;
private:
	Point BoradtoImg(int x, int y);
	int ImgtoBoradX(Point P);
	int ImgtoBoradY(Point P);
	bool mergeImg(Mat& dst, Mat& src, Point location);
};
