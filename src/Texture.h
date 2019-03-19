#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>


class Texture
{
	public:
		Texture();
		~Texture();

		void free();
		bool loadTexture(SDL_Renderer *renderer, std::string path);
		bool loadRendertext(SDL_Renderer *renderer, TTF_Font *font, std::string text, SDL_Color colour);
		void draw(SDL_Renderer *renderer, int x, int y, SDL_Rect *clip = NULL);

	private:
		SDL_Texture *t_texture;
		int t_width, t_height;
};