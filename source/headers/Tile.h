#pragma once
#include <SDL.h>


enum COLOUR { BLUE, GREEN, RED, YELLOW };


class Tile
{
	public:
		SDL_Rect rect;
		int numPawns[4];
		int index;
};