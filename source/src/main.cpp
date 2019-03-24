#include "Texture.h"
#include "Player.h"
#include "Board.h"

#undef main
#define fps 60


enum Turn { PLAYER1, PLAYER2 };
Turn turn = PLAYER1;

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

TTF_Font *font = nullptr;
Texture p1Text, p2Text;

Board board;
Player p1(board, 'b'), p2(board, 'g');

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

	SDL_Event ev;						//polled event
	int starting_time, ending_time;		//frame time
	int x, y;							//mouse states

	//Main loop
	while (true)
	{
		starting_time = SDL_GetTicks();

		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
				case SDL_QUIT:
					close();
					return 0;

				case SDL_KEYDOWN:
					switch (ev.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							close();
							return 0;

						case SDLK_z:
							if (turn == PLAYER1)
							{
								if (p1.numPawnsOut == 0 || p1.numPawnsOut > 1 || (p1.roll == 6 && p1.numPawnsHome > 0))
								{
									p1.choosing = true;
									break;
								}

								p1.rollDie(board);
							}
							break;

						case SDLK_x:
							if (turn == PLAYER2)
							{
								if (p2.numPawnsOut == 0 || p2.numPawnsOut > 1 || (p2.roll == 6 && p2.numPawnsHome > 0))
								{
									p2.choosing = true;
									break;
								}

								p2.rollDie(board);
							}
							break;
					}

					break;

				case SDL_MOUSEBUTTONDOWN:
					if (turn == PLAYER1 && p1.choosing)
					{
						SDL_GetMouseState(&x, &y);
						for (int i = 0; i < 4; i++)
						{
							if (x >= p1.pawn[i].p_currentPositionRect.x && x <= p1.pawn[i].p_currentPositionRect.x + p1.pawn[i].p_currentPositionRect.w
								&& y >= p1.pawn[i].p_currentPositionRect.y && y <= p1.pawn[i].p_currentPositionRect.y + p1.pawn[i].p_currentPositionRect.h)
							{
								if (p1.pawn[i].p_status != DONE)
									p1.currentPawn = i;

								p1.choosing = false;
								break;
							}
						}

						if (p1.currentPawn > -1)
							p1.rollDie(board);
					}

					else if (turn == PLAYER2 && p2.choosing)
					{
						SDL_GetMouseState(&x, &y);
						for (int i = 0; i < 4; i++)
						{
							if (x >= p2.pawn[i].p_currentPositionRect.x && x <= p2.pawn[i].p_currentPositionRect.x + p2.pawn[i].p_currentPositionRect.w
								&& y >= p2.pawn[i].p_currentPositionRect.y && y <= p2.pawn[i].p_currentPositionRect.y + p2.pawn[i].p_currentPositionRect.h)
							{
								if (p2.pawn[i].p_status != DONE)
									p2.currentPawn = i;

								p2.choosing = false;
								break;
							}
						}

						if (p2.currentPawn > -1)
							p2.rollDie(board);
					}

					break;
			}
		}

		//Draw game board
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		p1.update(board);
		p2.update(board);
		drawBoard();

		//Highlight pawns
		if (p1.choosing)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0xFF, 0xFF, 0xFF);
			for (int i = 0; i < 4; i++)
				if (p1.pawn[i].p_status != DONE)
					SDL_RenderDrawRect(renderer, &p1.pawn[i].p_currentPositionRect);
		}

		else if (p2.choosing)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0xFF, 0, 0);
			for (int i = 0; i < 4; i++)
				if (p2.pawn[i].p_status != DONE)
					SDL_RenderDrawRect(renderer, &p2.pawn[i].p_currentPositionRect);
		}

		//Change turns
		switch (turn)
		{
			case PLAYER1:
				if (p1.isDone)
					p1.isDone = false, turn = PLAYER2;
				break;

			case PLAYER2:
				if (p2.isDone)
					p2.isDone = false, turn = PLAYER1;
				break;
		}

		SDL_RenderPresent(renderer);

		//Cap frame rate
		ending_time = SDL_GetTicks();
		if (ending_time - starting_time < 1000 / fps)
			SDL_Delay(1000 / fps - (ending_time - starting_time));
	}

	close();
	return 0;
}


void initBoard()
{
	for (int i = 0; i < 4; i++)
	{
		switch (p1.colour)
		{
			case 'b':
				p1.pawn[i].p_body = { 0, 0, 40, 40 };
				p1.pawn[i].p_currentPositionRect = board.blueStartSquares[i];
				break;

			case 'g':
				p1.pawn[i].p_body = { 80, 0, 40, 40 };
				p1.pawn[i].p_currentPositionRect = board.greenStartSquares[i];
				break;
		}
		
		switch (p2.colour)
		{
			case 'b':
				p2.pawn[i].p_body = { 0, 0, 40, 40 };
				p2.pawn[i].p_currentPositionRect = board.blueStartSquares[i];
				break;

			case 'g':
				p2.pawn[i].p_body = { 80, 0, 40, 40 };
				p2.pawn[i].p_currentPositionRect= board.greenStartSquares[i];
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

	//Text
	p1Text.draw(renderer, 75, 565);
	p2Text.draw(renderer, 435, 15);

	//Pawns
	for (int i = 0; i < 4; i++)
	{
		pawnSheet.draw(renderer, p1.pawn[i].p_currentPositionRect.x, p1.pawn[i].p_currentPositionRect.y, &p1.pawn[i].p_body);
		pawnSheet.draw(renderer, p2.pawn[i].p_currentPositionRect.x, p2.pawn[i].p_currentPositionRect.y, &p2.pawn[i].p_body);
	}
}