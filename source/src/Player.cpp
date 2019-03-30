#include "Player.h"
#include "Board.h"
#include <cstdlib>
#include <ctime>


Player::Player(Board &board, char colour) : colourKey(colour)
{
	numPawnsOut = 0;
	numPawnsHome = 4;
	rolled = false;
	roll = 1;
	currentPawn = -1;
	isChoosing = false;
	isDoneMoving = false;

	switch (colour)
	{
		case 'b':
			pawn[0].p_startingPointRect = { 65, 490, 40, 40 };
			pawn[1].p_startingPointRect = { 135, 490, 40, 40 };
			pawn[2].p_startingPointRect = { 65, 425, 40, 40 };
			pawn[3].p_startingPointRect = { 135, 425, 40, 40 };
			diceRect = { 197, 557, 35, 35 };
			colour = BLUE;

			for (int i = 0; i < 4; i++)
			{
				pawn[i].p_startingTileNum = 2;
				pawn[i].p_endingTileNum = 0;
				pawn[i].p_colourNum = 0;

				for (int j = 0; j < 6; j++)
					pawn[i].p_home[j] = board.blueHome[j];

				pawn[i].p_form[0] = { 0, 0, 40, 40 };
				pawn[i].p_form[1] = { 0, 40, 40, 40 };
				pawn[i].p_form[2] = { 0, 80, 40, 40 };
				pawn[i].p_form[3] = { 0, 120, 40, 40 };
				pawn[i].p_currentPositionRect = board.blueStartSquares[i];
			}

			break;

		case 'g':
			pawn[0].p_startingPointRect = { 490, 65, 40, 40 };
			pawn[1].p_startingPointRect = { 490, 135, 40, 40 };
			pawn[2].p_startingPointRect = { 425, 65, 40, 40 };
			pawn[3].p_startingPointRect = { 425, 135, 40, 40 };
			diceRect = { 368, 8, 35, 35 };
			colour = GREEN;

			for (int i = 0; i < 4; i++)
			{
				pawn[i].p_startingTileNum = 28;
				pawn[i].p_endingTileNum = 26;
				pawn[i].p_colourNum = 1;

				for (int j = 0; j < 5; j++)
					pawn[i].p_home[j] = board.greenHome[j];

				pawn[i].p_form[0] = { 80, 0, 40, 40 };
				pawn[i].p_form[1] = { 80, 40, 40, 40 };
				pawn[i].p_form[2] = { 80, 80, 40, 40 };
				pawn[i].p_form[3] = { 80, 120, 40, 40 };
				pawn[i].p_currentPositionRect = board.blueStartSquares[i];
			}

			break;
	}

	for (int i = 0; i < 4; i++)
	{
		pawn[i].p_status = START;
		pawn[i].p_currentPositionRect = pawn[i].p_startingPointRect;
		pawn[i].p_endingTileRect = board.tile[pawn[i].p_endingTileNum].rect;
		pawn[i].p_currentTileNum = -1;
		pawn[i].p_numTilesMoved = 0;
		pawn[i].wasCaptured = false;
		pawn[i].canMove = true;
		pawn[i].p_currentForm = 0;
	}
}

void Player::pawnEscape(Board &board)
{
	pawn[currentPawn].p_currentPositionRect = board.tile[pawn[currentPawn].p_startingTileNum].rect;
	pawn[currentPawn].p_currentTileNum = pawn[currentPawn].p_startingTileNum;

	pawn[currentPawn].p_status = OUT;
	numPawnsOut++;
	numPawnsHome--;
}

void Player::rollDie(Board &board, int roll)
{
	srand(time(0));
	this->roll = (roll == NULL) ? 1 + rand() % 6 : roll;
	rolled = true;

	if ((roll == NULL && (numPawnsOut == 0 || numPawnsOut > 1 || (this->roll == 6 && numPawnsHome > 0))))
	{
		isChoosing = true;
		return;
	}

	for (int i = 0; i < 4; i++)
	{
		if (pawn[i].p_status == HOME && pawn[i].p_currentTileNum + this->roll > 5)
			pawn[i].canMove = false;

		else if (pawn[i].p_status == OUT || pawn[i].p_status == HOME)
			pawn[i].canMove = true;
	}

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
			{
				switch (numPawnsOut)
				{
					case 0:
						rolled = false;
						isDoneMoving = true;
						pawn[currentPawn].canMove = false;
						return;

					case 1:
						for (int i = 0; i < 4; i++)
							if (pawn[i].p_status == OUT)
								currentPawn = i;
						break;

					default:
						isChoosing = true;
						rolled = false;
						return;
				}

				pawn[currentPawn].canMove = false;
			}

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
			for (int i = 0; i < 4; i++)
				if (pawn[i].wasCaptured)
				{
					/*if (!(pawn[i].p_currentPositionRect.x == board.tile[pawn[i].p_startingTileNum].x &&
						pawn[i].p_currentPositionRect.y == board.tile[pawn[i].p_startingTileNum].y))
					{
						pawn[i].p_currentTileNum--;
						dest = pawn[i].p_currentTileNum;

						if ((dest >= 1 && dest <= 6) || (dest == 12 || dest == 13) || (dest >= 20 && dest <= 25))
						{
							if (dest == 6 && pawn[currentPawn].p_currentPositionRect.x != board.tile[dest].x)
								pawn[currentPawn].p_currentPositionRect.x += 10;

							if (pawn[currentPawn].p_currentPositionRect.y != board.tile[dest].y)
								pawn[currentPawn].p_currentPositionRect.y += 10;
						}

						else if ((dest >= 7 && dest <= 11) || (dest >= 40 && dest <= 45) || (dest == 51 || dest == 1))
						{
							if (dest == 45 && pawn[currentPawn].p_currentPositionRect.y != board.tile[dest].y)
								pawn[currentPawn].p_currentPositionRect.y -= 10;

							if (pawn[currentPawn].p_currentPositionRect.x != board.tile[dest].x)
								pawn[currentPawn].p_currentPositionRect.x += 10;
						}

						else if ((dest >= 14 && dest <= 19) || (dest == 26 || dest == 25) || (dest >= 32 && dest <= 37))
						{
							if (dest == 32 && pawn[currentPawn].p_currentPositionRect.y != board.tile[dest].x)
								pawn[currentPawn].p_currentPositionRect.y -= 10;

							if (pawn[currentPawn].p_currentPositionRect.x != board.tile[dest].x)
								pawn[currentPawn].p_currentPositionRect.x -= 10;
						}

						else if ((dest >= 27 && dest <= 32) || (dest == 39 || dest == 38) || (dest >= 46 && dest <= 50))
						{
							if (dest == 32 && pawn[currentPawn].p_currentPositionRect.x != board.tile[dest].x)
								pawn[currentPawn].p_currentPositionRect.x -= 10;

							if (pawn[currentPawn].p_currentPositionRect.y != board.tile[dest].y)
								pawn[currentPawn].p_currentPositionRect.y -= 10;
						}

						SDL_Delay(10);
					}

					else
					{*/
					pawn[i].p_currentPositionRect = pawn[i].p_startingPointRect;
					pawn[i].p_currentTileNum = -1;
					pawn[i].wasCaptured = false;
					pawn[i].p_status = START;
					numPawnsOut--;
					numPawnsHome++;
					return;
					//}
				}

			if (dest == pawn[currentPawn].p_endingTileNum + 1)
			{
				pawn[currentPawn].p_status = ENDINGTILE;
				break;
			}

			if ((dest >= 2 && dest <= 6) || (dest == 13 || dest == 14) || (dest >= 25 && dest <= 25))
			{
				if (pawn[currentPawn].p_currentPositionRect.y != board.tile[dest].rect.y)
					pawn[currentPawn].p_currentPositionRect.y -= 10;
			}

			else if ((dest >= 7 && dest <= 12) || (dest >= 41 && dest <= 45) || (dest == 0 || dest == 1))
			{
				if (dest == 7 && pawn[currentPawn].p_currentPositionRect.y != board.tile[dest].rect.y)
					pawn[currentPawn].p_currentPositionRect.y -= 10;

				if (pawn[currentPawn].p_currentPositionRect.x != board.tile[dest].rect.x)
					pawn[currentPawn].p_currentPositionRect.x -= 10;
			}

			else if ((dest >= 15 && dest <= 19) || (dest == 26 || dest == 27) || (dest >= 33 && dest <= 38))
			{
				if (dest == 33 && pawn[currentPawn].p_currentPositionRect.y != board.tile[dest].rect.y)
					pawn[currentPawn].p_currentPositionRect.y += 10;

				if (pawn[currentPawn].p_currentPositionRect.x != board.tile[dest].rect.x)
					pawn[currentPawn].p_currentPositionRect.x += 10;
			}

			else if ((dest >= 20 && dest <= 25))
			{
				if (dest == 20 && pawn[currentPawn].p_currentPositionRect.x != board.tile[dest].rect.x)
					pawn[currentPawn].p_currentPositionRect.x += 10;

				if (pawn[currentPawn].p_currentPositionRect.y != board.tile[dest].rect.y)
					pawn[currentPawn].p_currentPositionRect.y -= 10;
			}

			else if ((dest >= 28 && dest <= 32) || (dest == 39 || dest == 40) || (dest >= 46 && dest <= 51))
			{
				if (dest == 46 && pawn[currentPawn].p_currentPositionRect.x != board.tile[dest].rect.x)
					pawn[currentPawn].p_currentPositionRect.x -= 10;

				if (pawn[currentPawn].p_currentPositionRect.y != board.tile[dest].rect.y)
					pawn[currentPawn].p_currentPositionRect.y += 10;
			}


			//Reset tile number of pawn if it crosses the last tile
			else if (dest >= board.numTiles)
				pawn[currentPawn].p_currentTileNum = 0;


			//Render more than one pawn in a space
			for (int i = 0; i < 52; i++)
				board.tile[i].numPawns[colour] = 0;

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 52; j++)
				{
					if (pawn[i].p_currentTileNum == board.tile[j].index)
					{
						board.tile[j].numPawns[colour]++;
						break;
					}
				}
			}

			for (int i = 0; i < 4; i++)
				if (pawn[i].p_status == OUT)
					pawn[i].p_currentForm = board.tile[pawn[i].p_currentTileNum].numPawns[colour] - 1;
				else if (pawn[i].p_status == START)
					pawn[i].p_currentForm = 0;


			//Moving more than one tile
			if (pawn[currentPawn].p_currentPositionRect.x == board.tile[dest].rect.x && pawn[currentPawn].p_currentPositionRect.y == board.tile[dest].rect.y
				&& rolled && dest != pawn[currentPawn].p_startingTileNum && !isChoosing)
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
					isDoneMoving = true;
				}
			}

			break;

		case ENDINGTILE:
			pawn[currentPawn].p_currentTileNum = 0;
			pawn[currentPawn].p_status = HOME;
			isDoneMoving = true;
			break;

		case HOME:
			switch (colour)
			{
				case 'b':
					if (pawn[currentPawn].p_currentPositionRect.y != board.blueHome[dest].y)
						pawn[currentPawn].p_currentPositionRect.y -= 10;

					if (pawn[currentPawn].p_currentPositionRect.x == board.blueHome[5].x && pawn[currentPawn].p_currentPositionRect.y == board.blueHome[5].y)
					{
						numPawnsOut--;
						pawn[currentPawn].p_status = DONE;
						pawn[currentPawn].canMove = false;

						if (numPawnsOut == 1)
							for (int i = 0; i < 4; i++)
								if (pawn[i].p_status == OUT)
									currentPawn = i;
					}


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
							isDoneMoving = true;
						}
					}
					break;

				case 'g':
					if (pawn[currentPawn].p_currentPositionRect.y != board.greenHome[dest].y)
						pawn[currentPawn].p_currentPositionRect.y += 10;

					if (pawn[currentPawn].p_currentPositionRect.x == board.greenHome[5].x && pawn[currentPawn].p_currentPositionRect.y == board.greenHome[5].y)
					{
						numPawnsOut--;
						pawn[currentPawn].p_status = DONE;
						pawn[currentPawn].canMove = false;

						if (numPawnsOut == 1)
							for (int i = 0; i < 4; i++)
								if (pawn[i].p_status == OUT)
									currentPawn = i;
					}


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
							isDoneMoving = true;
						}
					}
					break;
			}
	}
}