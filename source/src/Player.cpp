#include "Player.h"
#include <cstdlib>
#include <ctime>


Player::Player(char colour) : colour(colour)
{
	numPawnsOut = 0;

	switch (colour)
	{
		case 'b':
			pawn[0].startingPoint = { 65, 490, 40, 40 };
			pawn[1].startingPoint = { 135, 490, 40, 40 };
			pawn[2].startingPoint = { 65, 425, 40, 40 };
			pawn[3].startingPoint = { 135, 425, 40, 40 };
			break;

		case 'g':
			pawn[0].startingPoint = { 490, 65, 40, 40 };
			pawn[1].startingPoint = { 490, 135, 40, 40 };
			pawn[2].startingPoint = { 425, 65, 40, 40 };
			pawn[3].startingPoint = { 425, 135, 40, 40 };
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

			pawn[0].currentPosition = board.tile[2];
			break;
	}
}