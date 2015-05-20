#ifndef MOVING_ENTITY_HPP
#define MOVING_ENTITY_HPP

#include "../GameObject/GameObject.hpp"
#include <SDL/SDL.h>

class MovingEntity: public GameObject{
	protected:

		double _speed;

	public:
		MovingEntity();
		virtual ~MovingEntity();

		double getSpeed() const;
		void setSpeed(double speed);

		void doubleSpeed();
		void halfSpeed();
};

#endif