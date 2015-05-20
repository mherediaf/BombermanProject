#ifndef FLAME_HPP
#define FLAME_HPP

class Flame;

#include "../TimedObject/TimedObject.hpp"
#include "../GameObject/GameObject.hpp"
#include "../Player/Player.hpp"
#include "../Window/Window.hpp"
#include "../Bomb/Bomb.hpp"

class Flame: public TimedObject, public GameObject{

	protected:
		bool isInsideXAxis(int posX) const;
		bool isInsideYAxis(int posY) const;

		Bomb* _bomb_from;

	public:
		Flame(int x, int y, _Window* screen, Bomb* bomb_from);
		~Flame();

		bool aPlayerIsTouching(Player* player) const;
		Bomb* getBombFrom() const;
};

#endif