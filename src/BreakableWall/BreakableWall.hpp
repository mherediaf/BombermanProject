#ifndef BREAKABLE_WALL_HPP
#define BREAKABLE_WALL_HPP

#include "../Wall/Wall.hpp"
//#include "../StatChanger/StatChanger.hpp"

class BreakableWall: public Wall{
	public:
		BreakableWall();
		~BreakableWall();

		bool canBeBroken() const;

};

#endif