#include "Texture.h"
#include "Player.h"
#include "Board.h"

#undef main
#define fps 60


SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

TTF_Font *font = nullptr;
Texture p1Text, p2Text;

Player p1('b'), p2('g');
Board board;

Texture pawnSheet;

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

	int starting_time, ending_time;

	while (running)
	{
		starting_time = SDL_GetTicks();

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

						case SDLK_d:
							p1.rollDie(board);
							break;
					}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		drawBoard();

		SDL_RenderPresent(renderer);

		ending_time = SDL_GetTicks();
		if (ending_time - starting_time < 1000 / fps)
			SDL_Delay(1000 / fps - (ending_time - starting_time));
	}

	close();
	return 0;
}


void initBoard()
{
	//Pawns
	for (int i = 0; i < 4; i++)
	{
		switch (p1.colour)
		{
			case 'b':
				p1.pawn[i].body = { 0, 0, 40, 40 };
				p1.pawn[i].currentPosition = board.blueStartSquares[i];
				break;

			case 'g':
				p1.pawn[i].body = { 80, 0, 40, 40 };
				p1.pawn[i].currentPosition = board.greenStartSquares[i];
				break;
		}
		
		switch (p2.colour)
		{
			case 'b':
				p2.pawn[i].body = { 0, 0, 40, 40 };
				p2.pawn[i].currentPosition = board.blueStartSquares[i];
				break;

			case 'g':
				p2.pawn[i].body = { 80, 0, 40, 40 };
				p2.pawn[i].currentPosition = board.greenStartSquares[i];
				break;
		}
	}
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
	if (!board.finish.loadTexture(renderer, "Images/Finish.png")) return false;
	if (!pawnSheet.loadTexture(renderer, "Images/Pawns.png")) return false;

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
	board.draw(renderer);

	//Draw Text
	p1Text.draw(renderer, 75, 565);
	p2Text.draw(renderer, 435, 15);

	//Draw Pawns
	for (int i = 0; i < 4; i++)
	{
		pawnSheet.draw(renderer, p1.pawn[i].currentPosition.x, p1.pawn[i].currentPosition.y, &p1.pawn[i].body);
		pawnSheet.draw(renderer, p2.pawn[i].currentPosition.x, p2.pawn[i].currentPosition.y, &p2.pawn[i].body);
	}
}