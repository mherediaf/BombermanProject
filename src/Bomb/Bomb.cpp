#include "Bomb.hpp"

#include <SDL/SDL.h>
#include <iostream>

using namespace std;

Bomb::Bomb(int ticks, int x, int y, int power, Player* PlayerGeneratedFrom, _Window* screen):
	TimedObject(ticks), SolidObject(){

		this->_activated = false;

		this->_power = power;
		this->_PlayerGeneratedFrom = PlayerGeneratedFrom;

		this->loadBMP("../Images/bomb.bmp");

		Uint32 colorkey = SDL_MapRGB(screen->getFormat(), 0xFF, 0, 0xFF);

		SDL_SetColorKey(this->getSprite(), SDL_SRCCOLORKEY, colorkey);

		this->setX(x);
		this->setY(y);

		this->playerHasLeftArea = false;
}

Bomb::~Bomb(){
	if (this->isActivated()){
		this->_PlayerGeneratedFrom->oneBombExploded();
	}
}


int Bomb::getPower() const{
	return this->_power;
}

bool Bomb::canTheMovingEntityBeHere(GameObject* entity){
	int x;
	int y;

	x = entity->getX() + 6;
	y = entity->getY() + 5;

	bool res=false;

	res = res || !this->isInsideXAxis(x);
	res = res || !this->isInsideYAxis(y);

	if (entity == (MovingEntity*)this->_PlayerGeneratedFrom){
		if (!this->playerHasLeftArea){
			if (res){
				this->playerLeftArea();

			} 
			res = true;
		} 
	}

	return res;
}

bool Bomb::isInsideXAxis(int posX){
	bool res;
	res = true;
	
	res = res && posX <= (this->getX()-10+30);
	res = res && (posX + 18) >= (this->getX()+7);

	return res;
}

bool Bomb::isInsideYAxis(int posY){
	bool res;
	res = true;
	
	res = res && posY <= (this->getY()-4+30);
	res = res && (posY + 21) >= (this->getY()+13);

	return res;
}

void Bomb::explode(){
	//cout << "[Bomb] bomb exploding" << endl;
	if (this->ticksLeft > 1){
		this->ticksLeft = 1;
	}
	//cout << "[Bomb] bomb explodes" << endl;
}

bool Bomb::isActivated() const{
	return this->_activated;
}

void Bomb::activate(){
	this->_activated = true;
}


bool Bomb::HasThePlayerLeftTheArea() const{
	return this->playerHasLeftArea;
}

Player* Bomb::playerThatGeneratedTheBomb() const{
	return this->_PlayerGeneratedFrom;
}

void Bomb::playerLeftArea(){
	this->playerHasLeftArea = true;	
}