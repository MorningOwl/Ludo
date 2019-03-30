#include "Board.h"


Board::Board()
{
	//Tiles
	int temp = 14;
	tile[0].rect = { 280, 560, 40, 40 };

	for (int i = 1; i <= 6; i++)
		tile[i].rect = { 240, temp * 40, 40, 40 }, temp--;

	temp = 5;
	for (int i = 7; i <= 12; i++)
		tile[i].rect = { temp * 40, 320, 40, 40 }, temp--;

	tile[13].rect = { 0, 280, 40, 40 };

	temp = 1;
	for (int i = 14; i <= 19; i++)
		tile[i].rect = { (temp - 1) * 40, 240, 40, 40 }, temp++;

	temp = 5;
	for (int i = 20; i <= 25; i++)
		tile[i].rect = { 240, temp * 40, 40, 40 }, temp--;

	tile[26].rect = { 280, 0, 40, 40 };

	temp = 0;
	for (int i = 27; i <= 32; i++)
		tile[i].rect = { 320, temp * 40, 40, 40 }, temp++;

	temp = 9;
	for (int i = 33; i <= 38; i++)
		tile[i].rect = { temp * 40, 240, 40, 40 }, temp++;

	tile[39].rect = { 560, 280, 40, 40 };

	temp = 14;
	for (int i = 40; i <= 45; i++)
		tile[i].rect = { temp * 40, 320, 40, 40 }, temp--;

	temp = 9;
	for (int i = 46; i <= 51; i++)
		tile[i].rect = { 320, temp * 40, 40, 40 }, temp++;

	for (int i = 0; i < 52; i++)
		tile[i].index = i;

	//Dice
	dice[1] = { 0, 0, 35, 35 };
	dice[2] = { 35, 0, 35, 35 };
	dice[3] = { 70, 0, 35, 35 };
	dice[4] = { 0, 35, 35, 35 };
	dice[5] = { 35, 35, 35, 35 };
	dice[6] = { 70, 35, 35, 35 };

	//Home
	temp = 13;
	for (int i = 0; i < 6; i++)
		blueHome[i] = { 280, temp * 40, 40, 40 }, temp--;

	temp = 1;
	for (int i = 0; i < 6; i++)
		redHome[i] = { temp * 40, 280, 40, 40 }, temp++;

	temp = 1;
	for (int i = 0; i < 6; i++)
		greenHome[i] = { 280, temp * 40, 40, 40 }, temp++;

	temp = 13;
	for (int i = 0; i < 6; i++)
		yellowHome[i] = { temp * 40, 280, 40, 40 }, temp--;


	//Starting Points
	blueStartSquares[0] = { 65, 490, 40, 40 };
	blueStartSquares[1] = { 135, 490, 40, 40 };
	blueStartSquares[2] = { 65, 425, 40, 40 };
	blueStartSquares[3] = { 135, 425, 40, 40 };

	greenStartSquares[0] = { 490, 65, 40, 40 };
	greenStartSquares[1] = { 490, 135, 40, 40 };
	greenStartSquares[2] = { 425, 65, 40, 40 };
	greenStartSquares[3] = { 425, 135, 40, 40 };


	//Number of Pawns on a Tile
	for (int i = 0; i < 52; i++)
		for (int j = 0; j < 4; j++)
			tile[i].numPawns[j] = 0;
}

void Board::draw(SDL_Renderer *renderer)
{
	//Draw Starting boxes
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &red);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &green);

	SDL_SetRenderDrawColor(renderer, 0, 122, 255, 255);
	SDL_RenderFillRect(renderer, &blue);

	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_RenderFillRect(renderer, &yellow);


	//Draw Start
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &blueStart);
	SDL_RenderFillRect(renderer, &redStart);
	SDL_RenderFillRect(renderer, &greenStart);
	SDL_RenderFillRect(renderer, &yellowStart);


	//Draw Home and Finish
	SDL_SetRenderDrawColor(renderer, 0, 122, 255, 255);
	for (int i = 0; i < 5; i++)
		SDL_RenderFillRect(renderer, &blueHome[i]);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	for (int i = 0; i < 5; i++)
		SDL_RenderFillRect(renderer, &redHome[i]);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	for (int i = 0; i < 5; i++)
		SDL_RenderFillRect(renderer, &greenHome[i]);

	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	for (int i = 0; i < 5; i++)
		SDL_RenderFillRect(renderer, &yellowHome[i]);

	finish.draw(renderer, 240, 240);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	for (int i = 0; i < 5; i++)
	{
		SDL_RenderDrawRect(renderer, &blueHome[i]);
		SDL_RenderDrawRect(renderer, &redHome[i]);
		SDL_RenderDrawRect(renderer, &greenHome[i]);
		SDL_RenderDrawRect(renderer, &yellowHome[i]);
	}


	//Draw Tiles
	SDL_SetRenderDrawColor(renderer, 0, 122, 255, 255);
	SDL_RenderDrawRect(renderer, &tile[0].rect);
	SDL_RenderFillRect(renderer, &tile[2].rect);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &tile[15].rect);
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &tile[28].rect);
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_RenderFillRect(renderer, &tile[41].rect);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	for (int i = 0; i < 52; i++)
		SDL_RenderDrawRect(renderer, &tile[i].rect);
}