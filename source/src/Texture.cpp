#include "Texture.h"


Texture::Texture()
{
	t_texture = nullptr;
	t_width = 0;
	t_height = 0;
}

Texture::~Texture()
{
	free();
}

void Texture::free()
{
	if (t_texture != nullptr)
	{
		SDL_DestroyTexture(t_texture);
		t_texture = nullptr;
		t_width = 0;
		t_height = 0;
	}
}

bool Texture::loadTexture(SDL_Renderer *renderer, std::string path)
{
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
	t_texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

	t_width = loadedSurface->w;
	t_height = loadedSurface->h;

	SDL_FreeSurface(loadedSurface);
	return t_texture != NULL;
}

void Texture::setColour(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(t_texture, red, green, blue);
}

bool Texture::loadRendertext(SDL_Renderer *renderer, TTF_Font *font, std::string text, SDL_Color colour)
{
	SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), colour);
	t_texture = SDL_CreateTextureFromSurface(renderer, textSurface);

	t_width = textSurface->w;
	t_height = textSurface->h;

	SDL_FreeSurface(textSurface);
	return t_texture != NULL;
}

void Texture::draw(SDL_Renderer *renderer, int x, int y, SDL_Rect *clip)
{
	SDL_Rect drawRect = { x, y, t_width, t_height };

	if (clip != NULL)
	{
		drawRect.w = clip->w;
		drawRect.h = clip->h;
	}

	SDL_RenderCopy(renderer, t_texture, clip, &drawRect);
}