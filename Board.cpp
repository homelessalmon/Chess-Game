#include "Board.h"
Board::Board()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			boardSituation[i][j] = NULL;
		}
	}
}

Board::Board(Player* players[2])
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			boardSituation[i][j] = NULL;
		}
	}
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < players[i]->OwningPiece.size(); j++)
		{
			boardSituation[players[i]->OwningPiece[j].posY][players[i]->OwningPiece[j].posX] = &players[i]->OwningPiece[j];
		}
	}
}
