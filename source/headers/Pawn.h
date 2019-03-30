#pragma once
#include "Texture.h"

enum Status { START, OUT, ENDINGTILE, HOME, DONE };


class Pawn
{
	public:
		Texture p_texture;
		int p_colourNum;

		SDL_Rect p_form[4];
		int p_currentForm;

		SDL_Rect p_home[6];
		Status p_status;

		SDL_Rect p_startingPointRect;
		SDL_Rect p_currentPositionRect;
		SDL_Rect p_endingTileRect;

		int p_numTilesMoved;
		int p_numPawnsInSpace;

		int p_currentTileNum;
		int p_startingTileNum, p_endingTileNum;

		bool wasCaptured;
		bool canMove;
};