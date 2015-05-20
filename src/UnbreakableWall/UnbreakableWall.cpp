#include "UnbreakableWall.hpp"

UnbreakableWall::UnbreakableWall() : Wall(){
	this->loadBMP("../Images/unbreakableWall.bmp");
}

UnbreakableWall::~UnbreakableWall(){

}

bool UnbreakableWall::canBeBroken() const{
	return false;
}