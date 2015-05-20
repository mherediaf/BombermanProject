#include "Flame.hpp"

Flame::Flame(int x, int y, _Window* screen, Bomb* bomb_from): TimedObject(15), GameObject(){
	this->setX(x);
	this->setY(y);

	this->loadBMP("../Images/fire.bmp");

	Uint32 colorkey = SDL_MapRGB(screen->getFormat(), 0xFF, 0, 0xFF);

	SDL_SetColorKey(this->getSprite(), SDL_SRCCOLORKEY, colorkey);

	this->_bomb_from = bomb_from;
}

Flame::~Flame(){

}

bool Flame::aPlayerIsTouching(Player* player) const{
	int x;
	int y;

	x = player->getX() + 4;
	y = player->getY() + 3;

	bool res=true;

	res = res && this->isInsideXAxis(x);
	res = res && this->isInsideYAxis(y);

	return res;
}

bool Flame::isInsideXAxis(int posX) const{
	bool res;
	res = true;
	
	res = res && posX <= (this->getX()-2+30);
	res = res && (posX + 18) >= (this->getX()+2);

	return res;
}

bool Flame::isInsideYAxis(int posY) const{
	bool res;
	res = true;
	
	res = res && posY <= (this->getY()-2+30);
	res = res && (posY + 24) >= (this->getY()+2);

	return res;
}

Bomb* Flame::getBombFrom() const{
	return this->_bomb_from;
}