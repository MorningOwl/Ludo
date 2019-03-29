#include "Texture.h"
#include "Player.h"
#include "Board.h"

#undef main
#define fps 60


enum Turn { PLAYER1, PLAYER2 };
Turn turn = PLAYER1;
enum State { ROLLING, CHOOSING };
State state = ROLLING;

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

TTF_Font *font = nullptr;
Texture p1Text, p2Text;

Board board;
Player p1(board, 'b'), p2(board, 'g');

Texture pawnSheet;
Texture dice[2];

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

						case SDLK_c:
							for (int i = 0; i < 4; i++)
								if (p2.pawn[i].p_status == OUT || p1.pawn[i].p_status == OUT)
									p2.pawn[i].wasCaptured = true, p1.pawn[i].wasCaptured = true;
					}

					break;

				case SDL_MOUSEBUTTONDOWN:
					switch (state)
					{
						case ROLLING:
							SDL_GetMouseState(&x, &y);
							if (turn == PLAYER1 && x > p1.diceRect.x && x < p1.diceRect.x + p1.diceRect.w
								&& y > p1.diceRect.y && y < p1.diceRect.y + p1.diceRect.h)
							{
								p1.rollDie(board);
								if (p1.isChoosing)
									state = CHOOSING;
							}

							else
							if (turn == PLAYER2 && x > p2.diceRect.x && x < p2.diceRect.x + p2.diceRect.w
								&& y > p2.diceRect.y && y < p2.diceRect.y + p2.diceRect.h)
							{
								p2.rollDie(board);
								if (p2.isChoosing)
									state = CHOOSING;
							}

							break;

						case CHOOSING:
							if (p1.isChoosing)
							{
								p1.currentPawn = -1;
								SDL_GetMouseState(&x, &y);
								for (int i = 0; i < 4; i++)
								{
									if (x >= p1.pawn[i].p_currentPositionRect.x && x <= p1.pawn[i].p_currentPositionRect.x + p1.pawn[i].p_currentPositionRect.w
										&& y >= p1.pawn[i].p_currentPositionRect.y && y <= p1.pawn[i].p_currentPositionRect.y + p1.pawn[i].p_currentPositionRect.h)
									{
										if (p1.pawn[i].canMove)
											p1.currentPawn = i;

										p1.isChoosing = false;
										state = ROLLING;
										break;
									}
								}

								if (p1.currentPawn > -1)
									p1.rollDie(board, p1.roll);
							}

							else if (p2.isChoosing)
							{
								p2.currentPawn = -1;
								SDL_GetMouseState(&x, &y);
								for (int i = 0; i < 4; i++)
								{
									if (x >= p2.pawn[i].p_currentPositionRect.x && x <= p2.pawn[i].p_currentPositionRect.x + p2.pawn[i].p_currentPositionRect.w
										&& y >= p2.pawn[i].p_currentPositionRect.y && y <= p2.pawn[i].p_currentPositionRect.y + p2.pawn[i].p_currentPositionRect.h)
									{
										if (p2.pawn[i].canMove)
											p2.currentPawn = i;

										p2.isChoosing = false;
										state = ROLLING;
										break;
									}
								}

								if (p2.currentPawn > -1)
									p2.rollDie(board, p2.roll);
							}

							break;
					}
			}
		}

		//Draw game board
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		p1.update(board);
		p2.update(board);
		drawBoard();

		//Highlight pawns
		if (p1.isChoosing)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0xFF, 0xFF, 0xFF);
			for (int i = 0; i < 4; i++)
				if (p1.pawn[i].canMove)
					SDL_RenderDrawRect(renderer, &p1.pawn[i].p_currentPositionRect);
		}

		else if (p2.isChoosing)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0xFF, 0, 0);
			for (int i = 0; i < 4; i++)
				if (p2.pawn[i].canMove)
					SDL_RenderDrawRect(renderer, &p2.pawn[i].p_currentPositionRect);
		}

		//Change turns
		switch (turn)
		{
			case PLAYER1:
				dice[PLAYER1].setColour(255, 255, 255);
				dice[PLAYER2].setColour(150, 150, 150);
				if (p1.isDoneMoving)
				{
					for (int i = 0; i < 4; i++)
						if (p1.pawn[p1.currentPawn].p_currentTileNum == p2.pawn[i].p_currentTileNum && p2.pawn[i].p_status == OUT)
							p2.pawn[i].wasCaptured = true;

					p1.isDoneMoving = false;
					turn = PLAYER2;
				}

				break;

			case PLAYER2:
				dice[PLAYER2].setColour(255, 255, 255);
				dice[PLAYER1].setColour(150, 150, 150);
				if (p2.isDoneMoving)
				{
					for (int i = 0; i < 4; i++)
						if (p2.pawn[p2.currentPawn].p_currentTileNum == p1.pawn[i].p_currentTileNum && p1.pawn[i].p_status == OUT)
							p1.pawn[i].wasCaptured = true;

					p2.isDoneMoving = false;
					turn = PLAYER1;
				}

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
				p1.pawn[i].p_form[0] = { 0, 0, 40, 40 };
				p1.pawn[i].p_form[1] = { 0, 40, 40, 40 };
				p1.pawn[i].p_form[2] = { 0, 80, 40, 40 };
				p1.pawn[i].p_form[3] = { 0, 120, 40, 40 };
				p1.pawn[i].p_currentPositionRect = board.blueStartSquares[i];
				break;

			case 'g':
				p1.pawn[i].p_form[0] = { 80, 0, 40, 40 };
				p1.pawn[i].p_form[1] = { 80, 40, 40, 40 };
				p1.pawn[i].p_form[2] = { 80, 80, 40, 40 };
				p1.pawn[i].p_form[3] = { 80, 120, 40, 40 };
				p1.pawn[i].p_currentPositionRect = board.blueStartSquares[i];
				break;
		}
		
		switch (p2.colour)
		{
			case 'b':
				p2.pawn[i].p_form[0] = { 0, 0, 40, 40 };
				p2.pawn[i].p_form[1] = { 0, 40, 40, 40 };
				p2.pawn[i].p_form[2] = { 0, 80, 40, 40 };
				p2.pawn[i].p_form[3] = { 0, 120, 40, 40 };
				p2.pawn[i].p_currentPositionRect = board.blueStartSquares[i];
				break;

			case 'g':
				p2.pawn[i].p_form[0] = { 80, 0, 40, 40 };
				p2.pawn[i].p_form[1] = { 80, 40, 40, 40 };
				p2.pawn[i].p_form[2] = { 80, 80, 40, 40 };
				p2.pawn[i].p_form[3] = { 80, 120, 40, 40 };
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
	if (!dice[0].loadTexture(renderer, "Images/Dice.png")) return false;
	if (!dice[1].loadTexture(renderer, "Images/Dice.png")) return false;

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
		pawnSheet.draw(renderer, p1.pawn[i].p_currentPositionRect.x, p1.pawn[i].p_currentPositionRect.y, &p1.pawn[i].p_form[p1.pawn[i].p_currentForm]);
		pawnSheet.draw(renderer, p2.pawn[i].p_currentPositionRect.x, p2.pawn[i].p_currentPositionRect.y, &p2.pawn[i].p_form[p2.pawn[i].p_currentForm]);
	}

	dice[PLAYER1].draw(renderer, 197, 557, &board.dice[p1.roll]);
	dice[PLAYER2].draw(renderer, 368, 8, &board.dice[p2.roll]);
}