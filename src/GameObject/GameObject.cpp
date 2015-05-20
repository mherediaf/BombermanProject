#include "GameObject.hpp"

GameObject::GameObject(): SpritedEntity(){
	this->myPosition = new Position();
}

GameObject::~GameObject(){
	delete this->myPosition;
}

int GameObject::getX() const{
	return (int)this->myPosition->getX();
}

int GameObject::getY() const{
	return (int)this->myPosition->getY();
}

void GameObject::setX(int x){
	this->myPosition->setX((double)x);
}

void GameObject::setY(int y){
	this->myPosition->setY((double)y);
}
