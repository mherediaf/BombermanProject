#ifndef SOLID_OBJECT_HPP
#define SOLID_OBJECT_HPP

#include "../GameObject/GameObject.hpp"

class SolidObject: public GameObject{
	protected:
		virtual bool isInsideXAxis(int posX)=0;
		virtual bool isInsideYAxis(int posY)=0;

	public:
		SolidObject();
		virtual ~SolidObject();

		virtual bool canTheMovingEntityBeHere(GameObject* entity)=0;
};

#endif
