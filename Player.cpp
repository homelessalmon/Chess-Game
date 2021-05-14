#include "Player.h"

Player::Player(int n)//n=0 white,1 black
{
	if (n == 0)
	{
		ChessPiece Pawn(0, Pawn, 0, 1), Queen(0, Queen, 3, 0), King(0, King, 4, 0), Rook(0, Rook, 0, 0), Bishop(0, Bishop, 0, 2), Knight(0, Knight, 0, 2);
		for (int i = 0; i < 8; i++) { OwningPiece.push_back(Pawn); Pawn.posX++; }
		for (int i = 0; i < 2; i++) { OwningPiece.push_back(Rook); Rook.posX += 7; }
		for (int i = 0; i < 2; i++) { OwningPiece.push_back(Knight); Knight.posX += 5; }
		for (int i = 0; i < 2; i++) { OwningPiece.push_back(Bishop); Bishop.posX += 3; }
		OwningPiece.push_back(Queen);
		OwningPiece.push_back(King);
	}
	else
	{
		ChessPiece Pawn(1, Pawn, 0, 6), Queen(1, Queen, 3, 7), King(1, King, 4, 7), Rook(1, Rook, 0, 7), Bishop(1, Bishop, 0, 7), Knight(1, Knight, 0, 7);
		for (int i = 0; i < 8; i++) { OwningPiece.push_back(Pawn); Pawn.posX++; }
		for (int i = 0; i < 2; i++) { OwningPiece.push_back(Rook); Rook.posX += 7; }
		for (int i = 0; i < 2; i++) { OwningPiece.push_back(Knight); Knight.posX += 5; }
		for (int i = 0; i < 2; i++) { OwningPiece.push_back(Bishop); Bishop.posX += 3; }
		OwningPiece.push_back(Queen);
		OwningPiece.push_back(King);
	}
}
