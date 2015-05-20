#ifndef WALL_HPP
#define WALL_HPP

#include "../SolidObject/SolidObject.hpp"
#include "../GameObject/GameObject.hpp"

class Wall: public SolidObject{
	protected:
		bool isInsideXAxis(int posX);
		bool isInsideYAxis(int posY);

	public:
		Wall();
		virtual ~Wall();

		virtual bool canBeBroken() const=0;
		bool canTheMovingEntityBeHere(GameObject* entity);

	
};

#endif
