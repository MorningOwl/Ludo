#include "Player.h"
#include "Board.h"
#include <cstdlib>
#include <ctime>


Player::Player(Board &board, char colour) : colour(colour)
{
	numPawnsOut = 0;
	numPawnsHome = 4;
	rolled = false;
	currentPawn = -1;
	choosing = false;

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
				pawn[i].p_currentPositionRect = pawn[i].p_startingPointRect;

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

void Player::pawnEscape(Board &board)
{
	pawn[currentPawn].p_currentPositionRect = board.tile[pawn[currentPawn].p_startingTileNum];
	pawn[currentPawn].p_currentTileNum = pawn[currentPawn].p_startingTileNum;

	pawn[currentPawn].p_status = OUT;
	numPawnsOut++;
	numPawnsHome--;
}

void Player::rollDie(Board &board)
{
	srand(time(0));
	roll = 1 + rand() % 6;
	rolled = true;

	switch (pawn[currentPawn].p_status)
	{
		case START:
			pawnEscape(board);
			break;

		case OUT:
			pawn[currentPawn].p_currentTileNum++;
			break;

		case HOME:
			if (pawn[currentPawn].p_currentTileNum + roll > 5)
				break;

			pawn[currentPawn].p_currentTileNum++;
			break;
	}
}

void Player::update(Board &board)
{
	int dest = pawn[currentPawn].p_currentTileNum;

	switch (pawn[currentPawn].p_status)
	{
		case OUT:
			if (dest == pawn[currentPawn].p_endingTileNum + 1)
			{
				pawn[currentPawn].p_status = ENDINGTILE;
				break;
			}

			if ((dest >= 2 && dest <= 6) || (dest == 13 || dest == 14) || (dest >= 25 && dest <= 25))
			{
				if (pawn[currentPawn].p_currentPositionRect.y != board.tile[dest].y)
					pawn[currentPawn].p_currentPositionRect.y -= 10;
			}

			else if ((dest >= 7 && dest <= 12) || (dest >= 41 && dest <= 45) || (dest == 0 || dest == 1))
			{
				if (dest == 7 && pawn[currentPawn].p_currentPositionRect.y != board.tile[dest].y)
					pawn[currentPawn].p_currentPositionRect.y -= 10;

				if (pawn[currentPawn].p_currentPositionRect.x != board.tile[dest].x)
					pawn[currentPawn].p_currentPositionRect.x -= 10;
			}

			else if ((dest >= 15 && dest <= 19) || (dest == 26 || dest == 27) || (dest >= 33 && dest <= 38))
			{
				if (dest == 33 && pawn[currentPawn].p_currentPositionRect.y != board.tile[dest].y)
					pawn[currentPawn].p_currentPositionRect.y += 10;

				if (pawn[currentPawn].p_currentPositionRect.x != board.tile[dest].x)
					pawn[currentPawn].p_currentPositionRect.x += 10;
			}

			else if ((dest >= 20 && dest <= 25))
			{
				if (dest == 20 && pawn[currentPawn].p_currentPositionRect.x != board.tile[dest].x)
					pawn[currentPawn].p_currentPositionRect.x += 10;

				if (pawn[currentPawn].p_currentPositionRect.y != board.tile[dest].y)
					pawn[currentPawn].p_currentPositionRect.y -= 10;
			}

			else if ((dest >= 28 && dest <= 32) || (dest == 39 || dest == 40) || (dest >= 46 && dest <= 51))
			{
				if (dest == 46 && pawn[currentPawn].p_currentPositionRect.x != board.tile[dest].x)
					pawn[currentPawn].p_currentPositionRect.x -= 10;

				if (pawn[currentPawn].p_currentPositionRect.y != board.tile[dest].y)
					pawn[currentPawn].p_currentPositionRect.y += 10;
			}

			else if (dest >= board.numTiles)
				pawn[currentPawn].p_currentTileNum = 0;


			//Moving more than one tile
			if (pawn[currentPawn].p_currentPositionRect.x == board.tile[dest].x && pawn[currentPawn].p_currentPositionRect.y == board.tile[dest].y
				&& rolled && dest != pawn[currentPawn].p_startingTileNum)
			{
				pawn[currentPawn].p_numTilesMoved++;
				if (pawn[currentPawn].p_numTilesMoved < roll)
				{
					SDL_Delay(50);
					pawn[currentPawn].p_currentTileNum++;
				}

				else
				{
					pawn[currentPawn].p_numTilesMoved = 0;
					rolled = false;
				}
			}

			break;

		case ENDINGTILE:
			pawn[currentPawn].p_currentTileNum = 0;
			pawn[currentPawn].p_status = HOME;

			break;

		case HOME:
			switch (colour)
			{
				case 'b':
					if (pawn[currentPawn].p_currentPositionRect.y != board.blueHome[dest].y)
						pawn[currentPawn].p_currentPositionRect.y -= 10;

					if (pawn[currentPawn].p_currentPositionRect.x == board.blueHome[5].x && pawn[currentPawn].p_currentPositionRect.y == board.blueHome[5].y)
						pawn[currentPawn].p_status = DONE;


					//Moving more than one tile
					if (pawn[currentPawn].p_currentPositionRect.x == board.blueHome[dest].x && pawn[currentPawn].p_currentPositionRect.y == board.blueHome[dest].y
						&& rolled && dest != pawn[currentPawn].p_startingTileNum)
					{
						pawn[currentPawn].p_numTilesMoved++;
						if (pawn[currentPawn].p_numTilesMoved < roll)
						{
							SDL_Delay(50);
							pawn[currentPawn].p_currentTileNum++;
						}

						else
						{
							pawn[currentPawn].p_numTilesMoved = 0;
							rolled = false;
						}
					}
					break;

				case 'g':
					if (pawn[currentPawn].p_currentPositionRect.y != board.greenHome[dest].y)
						pawn[currentPawn].p_currentPositionRect.y += 10;

					if (pawn[currentPawn].p_currentPositionRect.x == board.greenHome[5].x && pawn[currentPawn].p_currentPositionRect.y == board.greenHome[5].y)
						pawn[currentPawn].p_status = DONE;


					//Moving more than one tile
					if (pawn[currentPawn].p_currentPositionRect.x == board.greenHome[dest].x && pawn[currentPawn].p_currentPositionRect.y == board.greenHome[dest].y
						&& rolled && dest != pawn[currentPawn].p_startingTileNum)
					{
						pawn[currentPawn].p_numTilesMoved++;
						if (pawn[currentPawn].p_numTilesMoved < roll)
						{
							SDL_Delay(50);
							pawn[currentPawn].p_currentTileNum++;
						}

						else
						{
							pawn[currentPawn].p_numTilesMoved = 0;
							rolled = false;
						}
					}
					break;
			}
	}
}