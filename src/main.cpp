#include "Texture.h"
#undef main


SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

TTF_Font *font = nullptr;
Texture p1Text, p2Text;

SDL_Rect box[42] = { 0 };

SDL_Rect red = { 0, 0, 3030, 300 };
SDL_Rect green = { 300, 0, 300, 300 };
SDL_Rect blue = { 0, 300, 300, 300 };
SDL_Rect yellow = { 300, 300, 300, 300 };

SDL_Rect blueHome[5] = { 0 };
SDL_Rect redHome[5] = { 0 };
SDL_Rect greenHome[5] = { 0 };
SDL_Rect yellowHome[5] = { 0 };

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

	return true;
}

bool loadMedia()
{
	font = TTF_OpenFont("Fonts/Antaro.ttf", 28);
	if (font == NULL) return false;

	SDL_Color colour = { 0, 0, 0 };
	if (!p1Text.loadRendertext(renderer, font, "Player 1", colour)) return false;
	if (!p2Text.loadRendertext(renderer, font, "Player 2", colour)) return false;
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
	int temp = 14;
	box[0] = { 280, 560, 40, 40 };

	for (int i = 1; i <= 6; i++)
		box[i] = { 240, temp * 40, 40, 40 }, temp--;

	temp = 5;
	for (int i = 7; i <= 12; i++)
		box[i] = { temp * 40, 320, 40, 40 }, temp--;

	box[13] = { 0, 280, 40, 40 };

	temp = 1;
	for (int i = 14; i <= 19; i++)
		box[i] = { (temp - 1) * 40, 240, 40, 40 }, temp++;

	temp = 5;
	for (int i = 20; i <= 25; i++)
		box[i] = { 240, temp * 40, 40, 40 }, temp--;

	box[26] = { 280, 0, 40, 40 };

	temp = 0;
	for (int i = 27; i <= 32; i++)
		box[i] = { 320, temp * 40, 40, 40 }, temp++;

	temp = 9;
	for (int i = 33; i <= 38; i++)
		box[i] = { temp * 40, 240, 40, 40 }, temp++;

	box[39] = { 560, 280, 40, 40 };

	temp = 14;
	for (int i = 40; i <= 45; i++)
		box[i] = { temp * 40, 320, 40, 40 }, temp--;

	temp = 9;
	for (int i = 46; i <= 51; i++)
		box[i] = { 320, temp * 40, 40, 40 }, temp++;


	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	for (int i = 0; i < 52; i++)
		SDL_RenderDrawRect(renderer, &box[i]);
}