#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include "../SpritedEntity/SpritedEntity.hpp"
#include "../Position/Position.hpp"

class GameObject : public SpritedEntity{
	protected:
		Position* myPosition;

	public:
		GameObject();
		virtual ~GameObject();

		int getX() const;
		int getY() const;

		void setX(int x);
		void setY(int y);
};

#endif