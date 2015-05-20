#ifndef POSITION_HPP
#define POSITION_HPP

#include "../GameEntity/GameEntity.hpp"

class Position : public GameEntity{
	protected:
		double _x;
		double _y;

	public:
		Position();
		~Position();

		double getX() const;
		double getY() const;

		void setX(double x);
		void setY(double y);
};

#endif
