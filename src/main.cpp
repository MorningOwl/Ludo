#include "Texture.h"
#undef main


SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

TTF_Font *font = nullptr;
Texture p1Text, p2Text;
Texture finish;

SDL_Rect tile[52];

SDL_Rect red = { 0, 0, 240, 240 };
SDL_Rect green = { 360, 0, 240, 240 };
SDL_Rect blue = { 0, 360, 240, 240 };
SDL_Rect yellow = { 360, 360, 240, 240 };

SDL_Rect blueHome[5];
SDL_Rect redHome[5];
SDL_Rect greenHome[5];
SDL_Rect yellowHome[5];

SDL_Rect blueFinish = { 0, 0, 40, 40 };
SDL_Rect redFinish = { 0, 0, 40, 40 };
SDL_Rect greenFinish = { 0, 0, 40, 40 };
SDL_Rect yellowFinish = { 0, 0, 40, 40 };

void drawBoard();

bool init();
bool loadMedia();
void close();


int main(int argc, char *argv[])
{
	if (!(init() && loadMedia()))
	{
		close();
		return -1;
	}

	SDL_Event ev;
	bool running = true;

	while (running)
	{
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
				case SDL_QUIT:
					running = false;
					break;

				case SDL_KEYDOWN:
					switch (ev.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							running = false;
							break;
					}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		drawBoard();

		SDL_RenderPresent(renderer);
	}

	close();
	return 0;
}


void initBoard()
{
	//Tiles
	int temp = 14;
	tile[0] = { 280, 560, 40, 40 };

	for (int i = 1; i <= 6; i++)
		tile[i] = { 240, temp * 40, 40, 40 }, temp--;

	temp = 5;
	for (int i = 7; i <= 12; i++)
		tile[i] = { temp * 40, 320, 40, 40 }, temp--;

	tile[13] = { 0, 280, 40, 40 };

	temp = 1;
	for (int i = 14; i <= 19; i++)
		tile[i] = { (temp - 1) * 40, 240, 40, 40 }, temp++;

	temp = 5;
	for (int i = 20; i <= 25; i++)
		tile[i] = { 240, temp * 40, 40, 40 }, temp--;

	tile[26] = { 280, 0, 40, 40 };

	temp = 0;
	for (int i = 27; i <= 32; i++)
		tile[i] = { 320, temp * 40, 40, 40 }, temp++;

	temp = 9;
	for (int i = 33; i <= 38; i++)
		tile[i] = { temp * 40, 240, 40, 40 }, temp++;

	tile[39] = { 560, 280, 40, 40 };

	temp = 14;
	for (int i = 40; i <= 45; i++)
		tile[i] = { temp * 40, 320, 40, 40 }, temp--;

	temp = 9;
	for (int i = 46; i <= 51; i++)
		tile[i] = { 320, temp * 40, 40, 40 }, temp++;

	//Home
	temp = 13;
	for (int i = 0; i < 5; i++)
		blueHome[i] = { 280, temp * 40, 40, 40 }, temp--;

	temp = 1;
	for (int i = 0; i < 5; i++)
		redHome[i] = { temp * 40, 280, 40, 40 }, temp++;

	temp = 1;
	for (int i = 0; i < 5; i++)
		greenHome[i] = { 280, temp * 40, 40, 40 }, temp++;

	temp = 9;
	for (int i = 0; i < 5; i++)
		yellowHome[i] = { temp * 40, 280, 40, 40 }, temp++;
}

bool init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
		return false;

	if (TTF_Init() == -1)
		return false;

	window = SDL_CreateWindow("Ludo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_SHOWN);
	if (window == NULL) return false;

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) return false;

	initBoard();
	return true;
}

bool loadMedia()
{
	if (!finish.loadTexture(renderer, "Images/Finish.png")) return false;

	font = TTF_OpenFont("Fonts/Antaro.ttf", 28);
	if (font == NULL) return false;

	SDL_Color colour = { 0, 0, 0 };
	if (!p1Text.loadRendertext(renderer, font, "Player 1", colour)) return false;
	if (!p2Text.loadRendertext(renderer, font, "Player 2", colour)) return false;

	return true;
}

void close()
{
	TTF_CloseFont(font);
	font = nullptr;

	SDL_DestroyRenderer(renderer);
	renderer = nullptr;

	SDL_DestroyWindow(window);
	window = nullptr;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void drawBoard()
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

	//Draw Tiles
	SDL_SetRenderDrawColor(renderer, 0, 122, 255, 255);
	SDL_RenderFillRect(renderer, &tile[2]);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &tile[15]);
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &tile[28]);
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_RenderFillRect(renderer, &tile[41]);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	for (int i = 0; i < 52; i++)
		SDL_RenderDrawRect(renderer, &tile[i]);

	//Draw 
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	for (int i = 0; i < 5; i++)
	{
		SDL_RenderDrawRect(renderer, &blueHome[i]);
		SDL_RenderDrawRect(renderer, &redHome[i]);
		SDL_RenderDrawRect(renderer, &greenHome[i]);
		SDL_RenderDrawRect(renderer, &yellowHome[i]);
	}
}