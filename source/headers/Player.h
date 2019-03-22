#pragma once
#include "Pawn.h"
#include "Board.h"


class Player
{
	public:
		Player(Board &board, char colour);
		void rollDie(Board &board);
		void update(Board &board);

		Pawn pawn[4];
		int numPawnsOut;
		int roll;
		char colour;

		bool rollDice;
};