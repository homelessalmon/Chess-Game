#include "GameManager.h"
#include <vector>
#include <opencv2/opencv.hpp>
#include "ChessPiece.h"
using namespace std;
using namespace cv;

void GameManager::exe() {
	viewer.drawBoard();
	ChessPiece A(1, Queen, 5, 4);
	testPiece.push_back(A);
	for (int i = 0; i < testPiece.size(); i++) {
		viewer.drawChess(testPiece[i]);
	}
	namedWindow("Chess Game", WINDOW_AUTOSIZE);
	imshow("Chess Game", viewer.Screen);
	setMouseCallback("Chess Game", Viewer::mouseCallback, 0);
}
