#include "Player.h"
#include "Board.h"
#include <cstdlib>
#include <ctime>


Player::Player(Board &board, char colour) : colour(colour)
{
	numPawnsOut = 0;

	for (int i = 0; i < 4; i++)
	{
		pawn[i].isHome = false;
		pawn[i].isDone = false;
	}

	switch (colour)
	{
		case 'b':
			pawn[0].startingPoint = { 65, 490, 40, 40 };
			pawn[1].startingPoint = { 135, 490, 40, 40 };
			pawn[2].startingPoint = { 65, 425, 40, 40 };
			pawn[3].startingPoint = { 135, 425, 40, 40 };

			for (int i = 0; i < 4; i++)
			{
				pawn[i].startingTile = 2;
				pawn[i].endingTile = 52;

				for (int j = 0; j < 6; j++)
					pawn[i].home[j] = board.blueHome[j];
			}

			break;

		case 'g':
			pawn[0].startingPoint = { 490, 65, 40, 40 };
			pawn[1].startingPoint = { 490, 135, 40, 40 };
			pawn[2].startingPoint = { 425, 65, 40, 40 };
			pawn[3].startingPoint = { 425, 135, 40, 40 };

			for (int i = 0; i < 4; i++)
			{
				pawn[i].startingTile = 28;

				for (int j = 0; j < 5; j++)
					pawn[i].home[j] = board.greenHome[j];
			}

			break;
	}
}

void Player::rollDie(Board &board)
{
	srand(time(0));
	int roll = 1 + rand() % 6;

	switch (numPawnsOut)
	{
		case 0:
			if (roll != 6)
				return;

			pawn[0].currentTile = pawn[0].startingTile;
			numPawnsOut++;
			pawn[0].currentPosition = board.tile[pawn[0].currentTile];
			break;

		default:
			if (pawn[0].currentTile + roll >= pawn[0].endingTile || pawn[0].isHome)
			{
				if (pawn[0].isHome)
				{
					if (pawn[0].currentTile + roll > 5)
						break;

					pawn[0].currentTile += roll;
					pawn[0].currentPosition = pawn[0].home[pawn[0].currentTile];
					pawn[0].isDone = true;
					numPawnsOut--;
					break;
				}

				pawn[0].isHome = true;
				pawn[0].currentTile = pawn[0].currentTile + roll - pawn[0].endingTile;
				pawn[0].currentPosition = pawn[0].home[pawn[0].currentTile];
				break;
			}

			pawn[0].currentTile += roll;
			pawn[0].currentPosition = board.tile[pawn[0].currentTile];
			break;
	}
}