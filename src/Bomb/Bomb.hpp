#ifndef BOMB_HPP
#define BOMB_HPP

class Bomb;
#include "../Window/Window.hpp"
#include "../TimedObject/TimedObject.hpp"
#include "../SpritedEntity/SpritedEntity.hpp"
#include "../SolidObject/SolidObject.hpp"
#include "../Player/Player.hpp"
#include "../GameObject/GameObject.hpp"
//#include "../Field/Field.hpp"

class Bomb : public TimedObject, public SolidObject{
	private:
		bool isInsideXAxis(int posX);
		bool isInsideYAxis(int posY);

		int _power;
		bool _activated;
		bool playerHasLeftArea;

		Player* _PlayerGeneratedFrom;
		//Field* _fieldBelongingTo;
	public:
		Bomb(int ticks, int x, int y, int power, Player* PlayerGeneratedFrom, _Window* screen);
		~Bomb();

		int getPower() const;

		bool canTheMovingEntityBeHere(GameObject* entity);

		void explode();

		void activate();
		bool isActivated() const;

		bool HasThePlayerLeftTheArea() const;
		Player* playerThatGeneratedTheBomb() const;
		void playerLeftArea();
};

#endif