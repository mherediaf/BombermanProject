#include "BreakableWall.hpp"
#include <stdlib.h>

BreakableWall::BreakableWall() : Wall(){
	this->loadBMP("../Images/breakableWall.bmp");
}

BreakableWall::~BreakableWall(){

}

bool BreakableWall::canBeBroken() const{
	return true;
}
