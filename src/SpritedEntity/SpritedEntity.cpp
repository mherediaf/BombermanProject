#include "SpritedEntity.hpp"
#include <iostream>

#include "../Window/Window.hpp"

using namespace std;

extern _Window* screen;

SpritedEntity::SpritedEntity(): GameEntity(){
	this->image_set = false;
	this->spritePath = new string;
	//cout << "Sprited entity created" << endl;
}

SpritedEntity::~SpritedEntity(){
	if (this->imageIsSet() ){
		SDL_FreeSurface(this->sprite);
	}

	delete this->spritePath;
}

SDL_Surface* SpritedEntity::getSprite(){
	return this->sprite;
}

void SpritedEntity::loadBMP(char* path){
	this->sprite = SDL_LoadBMP(path);
	this->image_set = true;

	*(this->spritePath) = path;
}

void SpritedEntity::loadBMP(const char* path){
	this->sprite = SDL_LoadBMP(path);
	Uint32 colorkey = SDL_MapRGB(screen->getFormat(), 0xFF, 0, 0xFF);
//	cout << "Setting color key" << endl;

	SDL_SetColorKey(this->getSprite(), SDL_SRCCOLORKEY, colorkey);
//	cout << "Player dead" << endl;
	this->image_set = true;

	*(this->spritePath) = path;
}

void SpritedEntity::loadBMP(string& path){
	this->loadBMP(path.c_str());
}

string* SpritedEntity::getSpritePath() const{
	return this->spritePath;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////     PRIVATE FUNCTIONS     //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool SpritedEntity::imageIsSet(){
	return this->image_set;
}