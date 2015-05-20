#include "Position.hpp"

Position::Position(){
	this->setX(0.0);
	this->setY(0.0);
}

Position::~Position(){
}

double Position::getX() const{
	return this->_x;
}

double Position::getY() const{
	return this->_y;
}

void Position::setX(double x){
	this->_x = x;
}

void Position::setY(double y){
	this->_y = y;
}
