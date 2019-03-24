#pragma once
#include "Pawn.h"
#include "Board.h"


class Player
{
	public:
		Player(Board &board, char colour);
		void rollDie(Board &board);
		void pawnEscape(Board &board);
		void update(Board &board);

		Pawn pawn[4];
		int numPawnsOut;
		int numPawnsHome;

		int roll;
		char colour;

		bool rolled;
		int currentPawn;
		bool choosing;
		bool isDone;
};