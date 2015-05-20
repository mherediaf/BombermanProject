#include <iostream>
#include "Window.hpp"

using namespace std;

_Window::_Window(int w, int h): SpritedEntity(){
	this->screen = SDL_SetVideoMode(w, h, 32, SDL_SWSURFACE);
	cout << "Window initialized correctly" << endl;
}

_Window::~_Window(){
	SDL_FreeSurface(this->screen);
}

void _Window::setPosition(int x, int y){
}

void _Window::addLayer (SDL_Surface* image, SDL_Rect* cut, SDL_Rect* position){
	if (SDL_BlitSurface(image, cut, this->screen, position) != 0){
		cout << "Error blitting: " << SDL_GetError() << endl;
	}
}

void _Window::show(){
	SDL_Flip(this->screen);
}

const SDL_PixelFormat* _Window::getFormat(){
	return this->screen->format;
}