#include "TimedObject.hpp"

#include <iostream>

using namespace std;

TimedObject::TimedObject(int ticks): GameEntity(){
	this->ticksLeft = ticks;
}

TimedObject::~TimedObject(){
}

void TimedObject::tick(){
	//cout << "Trying to tick" << endl;
	if (!this->mustBeDestroyed()){
	//	cout << "Still not exploding. lowering 1 tick" << endl;
		this->ticksLeft--;
	}
	//cout << "Finished ticking" << endl;
}

bool TimedObject::mustBeDestroyed() const{
	//cout << "checking if has to explode" << endl;
	return this->ticksLeft == 0;
}

int TimedObject::amountOfTicksLeft() const{
	return this->ticksLeft;
}

void TimedObject::setTicks(int _ticks){
	this->ticksLeft = _ticks;
}