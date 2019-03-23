#pragma once
#include "Texture.h"

enum Status { START, OUT, ENDINGTILE, HOME, DONE };

class Pawn
{
	public:
		Texture p_texture;
		SDL_Rect p_body;
		SDL_Rect p_home[6];
		Status p_status;

		SDL_Rect p_startingPointRect;
		SDL_Rect p_currentPositionRect;
		SDL_Rect p_endingTileRect;

		int p_numTilesMoved;

		int p_currentTileNum;
		int p_startingTileNum, p_endingTileNum;
};