#pragma once
#include "Texture.h"


class Pawn
{
	public:
		Texture p_texture;
		SDL_Rect body;
		SDL_Rect startingPoint;
		SDL_Rect currentPosition;
};