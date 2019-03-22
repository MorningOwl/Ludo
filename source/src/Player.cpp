#include "Player.h"
#include "Board.h"
#include <cstdlib>
#include <ctime>


Player::Player(Board &board, char colour) : colour(colour)
{
	numPawnsOut = 0;
	rollDice = false;

	switch (colour)
	{
		case 'b':
			pawn[0].p_startingPointRect = { 65, 490, 40, 40 };
			pawn[1].p_startingPointRect = { 135, 490, 40, 40 };
			pawn[2].p_startingPointRect = { 65, 425, 40, 40 };
			pawn[3].p_startingPointRect = { 135, 425, 40, 40 };

			for (int i = 0; i < 4; i++)
			{
				pawn[i].p_startingTileNum = 2;
				pawn[i].p_endingTileNum = 0;

				for (int j = 0; j < 6; j++)
					pawn[i].p_home[j] = board.blueHome[j];
			}

			break;

		case 'g':
			pawn[0].p_startingPointRect = { 490, 65, 40, 40 };
			pawn[1].p_startingPointRect = { 490, 135, 40, 40 };
			pawn[2].p_startingPointRect = { 425, 65, 40, 40 };
			pawn[3].p_startingPointRect = { 425, 135, 40, 40 };

			for (int i = 0; i < 4; i++)
			{
				pawn[i].p_startingTileNum = 28;
				pawn[i].p_endingTileNum = 26;

				for (int j = 0; j < 5; j++)
					pawn[i].p_home[j] = board.greenHome[j];
			}

			break;
	}

	for (int i = 0; i < 4; i++)
	{
		pawn[i].p_status = START;
		pawn[i].p_endingTileRect = board.tile[pawn[i].p_endingTileNum];
		pawn[i].p_numTilesMoved = 0;
	}
}

//void Player::rollDie(Board &board)
//{
//	srand(time(0));
//	roll = 1;// +rand() % 3;
//
//	switch (numPawnsOut)
//	{
//		case 0:
//			//if (roll != 6)
//			//	return;
//
//			pawn[0].p_currentTileNum = pawn[0].p_startingTileNum;
//			pawn[0].p_destination = pawn[0].p_startingTileNum;
//
//			pawn[0].p_isOut = true;
//			numPawnsOut++;
//			pawn[0].p_currentPositionRect = board.tile[pawn[0].p_currentTileNum];
//			break;
//
//		default:
//			if (pawn[0].p_currentTileNum + roll >= pawn[0].p_endingTileNum || pawn[0].p_isHome)
//			{
//				if (pawn[0].p_isHome)
//				{
//					if (pawn[0].p_currentTileNum + roll > 5)
//						break;
//
//					pawn[0].p_currentTileNum += roll;
//					pawn[0].p_currentPositionRect = pawn[0].p_home[pawn[0].p_currentTileNum];
//					pawn[0].p_isDone = true;
//					numPawnsOut--;
//					break;
//				}
//
//				pawn[0].p_isHome = true;
//				pawn[0].p_currentTileNum = pawn[0].p_currentTileNum + roll - pawn[0].p_endingTileNum;
//				pawn[0].p_currentPositionRect = pawn[0].p_home[pawn[0].p_currentTileNum];
//				break;
//			}
//
//			pawn[0].p_destination = pawn[0].p_currentTileNum + roll;
//
//			//pawn[0].p_currentTileNum += roll;
//			//pawn[0].currentPosition = board.tile[pawn[0].p_currentTileNum];
//			break;
//	}
//}

void Player::rollDie(Board &board)
{
	srand(time(0));
	roll = 1 + rand() % 6;
	rollDice = true;

	switch (numPawnsOut)
	{
		case 0:
			//if (roll != 6)
			//	return;

			pawn[0].p_currentPositionRect = board.tile[pawn[0].p_startingTileNum];
			pawn[0].p_currentTileNum = pawn[0].p_startingTileNum;

			pawn[0].p_status = OUT;
			numPawnsOut++;
			break;

		case 1:
			switch (pawn[0].p_status)
			{
				case OUT:
					pawn[0].p_currentTileNum++;
					break;

				case HOME:
					pawn[0].p_currentTileNum++;
					break;
			}

			break;
	}
}

void Player::update(Board &board)
{
	int dest = pawn[0].p_currentTileNum;

	switch (pawn[0].p_status)
	{
		case OUT:
			if (dest == pawn[0].p_endingTileNum + 1)
			{
				pawn[0].p_status = ENDINGTILE;
				break;
			}

			if ((dest >= 2 && dest <= 6) || (dest == 13 || dest == 14) || (dest >= 25 && dest <= 25))
			{
				if (pawn[0].p_currentPositionRect.y != board.tile[dest].y)
					pawn[0].p_currentPositionRect.y -= 10;
			}

			else if ((dest >= 7 && dest <= 12) || (dest >= 41 && dest <= 45) || (dest == 0 || dest == 1))
			{
				if (dest == 7 && pawn[0].p_currentPositionRect.y != board.tile[dest].y)
					pawn[0].p_currentPositionRect.y -= 10;

				if (pawn[0].p_currentPositionRect.x != board.tile[dest].x)
					pawn[0].p_currentPositionRect.x -= 10;
			}

			else if ((dest >= 15 && dest <= 19) || (dest == 26 || dest == 27) || (dest >= 33 && dest <= 38))
			{
				if (dest == 33 && pawn[0].p_currentPositionRect.y != board.tile[dest].y)
					pawn[0].p_currentPositionRect.y += 10;

				if (pawn[0].p_currentPositionRect.x != board.tile[dest].x)
					pawn[0].p_currentPositionRect.x += 10;
			}

			else if ((dest >= 20 && dest <= 25))
			{
				if (dest == 20 && pawn[0].p_currentPositionRect.x != board.tile[dest].x)
					pawn[0].p_currentPositionRect.x += 10;

				if (pawn[0].p_currentPositionRect.y != board.tile[dest].y)
					pawn[0].p_currentPositionRect.y -= 10;
			}

			else if ((dest >= 28 && dest <= 32) || (dest == 39 || dest == 40) || (dest >= 46 && dest <= 51))
			{
				if (dest == 46 && pawn[0].p_currentPositionRect.x != board.tile[dest].x)
					pawn[0].p_currentPositionRect.x -= 10;

				if (pawn[0].p_currentPositionRect.y != board.tile[dest].y)
					pawn[0].p_currentPositionRect.y += 10;
			}

			else if (dest >= board.numTiles)
				pawn[0].p_currentTileNum = 0;


			//Moving more than one tile
			if (pawn[0].p_currentPositionRect.x == board.tile[dest].x && pawn[0].p_currentPositionRect.y == board.tile[dest].y
				&& rollDice && dest != pawn[0].p_startingTileNum)
			{
				pawn[0].p_numTilesMoved++;
				if (pawn[0].p_numTilesMoved < roll)
				{
					SDL_Delay(50);
					pawn[0].p_currentTileNum++;
				}

				else
				{
					pawn[0].p_numTilesMoved = 0;
					rollDice = false;
				}
			}

			break;

		case ENDINGTILE:
			//if (dest == pawn[0].p_endingTileNum + 1)
			{
				pawn[0].p_currentTileNum = 0;
				pawn[0].p_status = HOME;
			}

			break;

		case HOME:
			switch (colour)
			{
				case 'b':
					if (pawn[0].p_currentPositionRect.y != board.blueHome[dest].y)
						pawn[0].p_currentPositionRect.y -= 10;

					if (pawn[0].p_currentPositionRect.x == board.blueHome[5].x && pawn[0].p_currentPositionRect.y == board.blueHome[5].y)
						pawn[0].p_status = DONE;


					//Moving more than one tile
					if (pawn[0].p_currentPositionRect.x == board.blueHome[dest].x && pawn[0].p_currentPositionRect.y == board.blueHome[dest].y
						&& rollDice && dest != pawn[0].p_startingTileNum)
					{
						pawn[0].p_numTilesMoved++;
						if (pawn[0].p_numTilesMoved < roll)
						{
							SDL_Delay(50);
							pawn[0].p_currentTileNum++;
						}

						else
						{
							pawn[0].p_numTilesMoved = 0;
							rollDice = false;
						}
					}
					break;

				case 'g':
					if (pawn[0].p_currentPositionRect.y != board.greenHome[dest].y)
						pawn[0].p_currentPositionRect.y += 10;

					if (pawn[0].p_currentPositionRect.x == board.greenHome[5].x && pawn[0].p_currentPositionRect.y == board.greenHome[5].y)
						pawn[0].p_status = DONE;


					//Moving more than one tile
					if (pawn[0].p_currentPositionRect.x == board.greenHome[dest].x && pawn[0].p_currentPositionRect.y == board.greenHome[dest].y
						&& rollDice && dest != pawn[0].p_startingTileNum)
					{
						pawn[0].p_numTilesMoved++;
						if (pawn[0].p_numTilesMoved < roll)
						{
							SDL_Delay(50);
							pawn[0].p_currentTileNum++;
						}

						else
						{
							pawn[0].p_numTilesMoved = 0;
							rollDice = false;
						}
					}
					break;
			}
	}
}