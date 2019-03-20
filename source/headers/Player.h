#pragma once
#include "Pawn.h"
#include "Board.h"


class Player
{
	public:
		Player(char colour);
		void rollDie(Board &board);

		Pawn pawn[4];
		int numPawnsOut;
		char colour;
};