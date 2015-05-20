#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL/SDL.h>

#include "../SpritedEntity/SpritedEntity.hpp"

class _Window: public SpritedEntity{
	protected:
		SDL_Surface* screen;

	public:
		_Window(int w, int h);
		~_Window();

		void setPosition(int x, int y);

		void addLayer (SDL_Surface* image, SDL_Rect* cut, SDL_Rect* position);

		void show();

		const SDL_PixelFormat* getFormat();
};

#endif