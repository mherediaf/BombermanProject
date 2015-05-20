#ifndef UNBREAKABLE_WALL_HPP
#define UNBREAKABLE_WALL_HPP

#include "../Wall/Wall.hpp"

class UnbreakableWall: public Wall{
	public:
		UnbreakableWall();
		~UnbreakableWall();

		bool canBeBroken() const;
};

#endif