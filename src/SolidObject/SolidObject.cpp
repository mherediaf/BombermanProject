#include "SolidObject.hpp"

SolidObject::SolidObject(): GameObject(){
}

SolidObject::~SolidObject(){

}

/*
bool SolidObject::isInsideXAxis(int posX){
	bool res;
	res = true;
	
	res = res && posX <= (this->getX()-2+30);
	res = res && (posX + 30) >= (this->getX()+2);

	return res;
}

bool SolidObject::isInsideYAxis(int posY){
	bool res;
	res = true;
	
	res = res && posY <= (this->getY()-2+30);
	res = res && (posY + 30) >= (this->getY()+2);

	return res;
}
*/