#include "Wall.hpp"

Wall::Wall(): SolidObject(){

}

Wall::~Wall(){
}

bool Wall::canTheMovingEntityBeHere(GameObject* entity){
	int x;
	int y;

	x = entity->getX() + 4;
	y = entity->getY() + 3;

	bool res=false;

	res = res || !this->isInsideXAxis(x);
	res = res || !this->isInsideYAxis(y);

	return res;
}

bool Wall::isInsideXAxis(int posX){
	bool res;
	res = true;
	
	res = res && posX <= (this->getX()-2+30);
	res = res && (posX + 20) >= (this->getX()+2);

	return res;
}

bool Wall::isInsideYAxis(int posY){
	bool res;
	res = true;
	
	res = res && posY <= (this->getY()-2+30);
	res = res && (posY + 24) >= (this->getY()+2);

	return res;
}