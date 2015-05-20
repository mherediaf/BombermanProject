#ifndef TIMED_OBJECT_HPP
#define TIMED_OBJECT_HPP

#include "../GameEntity/GameEntity.hpp"
#include "../Position/Position.hpp"

class TimedObject: public GameEntity{
	protected:
		int ticksLeft;

	public:
		TimedObject(int ticks);
		virtual ~TimedObject();

		void tick();
		bool mustBeDestroyed() const;

		int amountOfTicksLeft() const;

		void setTicks(int _ticks);

};

#endif