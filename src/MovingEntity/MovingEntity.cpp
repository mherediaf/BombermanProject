#include "MovingEntity.hpp"

#include <iostream>

using namespace std;

MovingEntity::MovingEntity(): GameObject(){
	//cout << "Moving entity created" << endl;

}

MovingEntity::~MovingEntity(){
}

double MovingEntity::getSpeed() const{
	return this->_speed;
}

void MovingEntity::setSpeed(double speed){
	this->_speed = speed;
}

void MovingEntity::doubleSpeed(){
	this->setSpeed(this->getSpeed() * 2.0);
}

void MovingEntity::halfSpeed(){
	this->setSpeed(this->getSpeed() / 2.0);
}
