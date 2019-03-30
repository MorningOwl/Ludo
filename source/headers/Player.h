#pragma once
#include "Pawn.h"
#include "Board.h"


class Player
{
	public:
		Player(Board &board, char colour);
		void rollDie(Board &board, int roll = NULL);
		void pawnEscape(Board &board);
		void update(Board &board);

		Pawn pawn[4];
		int numPawnsOut;
		int numPawnsHome;

		SDL_Rect diceRect;

		int roll;
		char colourKey;
		COLOUR colour;

		bool rolled;
		int currentPawn;
		bool isChoosing;
		bool isDoneMoving;
};