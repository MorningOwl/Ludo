#pragma once
#include <SDL.h>
#include "Texture.h"


class Board
{
	public:
		Board();
		void draw(SDL_Renderer *renderer);

		SDL_Rect tile[52];
		Texture finish;

		SDL_Rect redStartSquares[4];
		SDL_Rect greenStartSquares[4];
		SDL_Rect blueStartSquares[4];
		SDL_Rect yellowStartSquares[4];

		SDL_Rect red = { 0, 0, 240, 240 };
		SDL_Rect green = { 360, 0, 240, 240 };
		SDL_Rect blue = { 0, 360, 240, 240 };
		SDL_Rect yellow = { 360, 360, 240, 240 };

		SDL_Rect redStart = { 50, 50, 140, 140 };
		SDL_Rect greenStart = { 410, 50, 140, 140 };
		SDL_Rect blueStart = { 50, 410, 140, 140 };
		SDL_Rect yellowStart = { 410, 410, 140, 140 };

		SDL_Rect blueHome[5];
		SDL_Rect redHome[5];
		SDL_Rect greenHome[5];
		SDL_Rect yellowHome[5];

		SDL_Rect blueFinish = { 0, 0, 40, 40 };
		SDL_Rect redFinish = { 0, 0, 40, 40 };
		SDL_Rect greenFinish = { 0, 0, 40, 40 };
		SDL_Rect yellowFinish = { 0, 0, 40, 40 };
};