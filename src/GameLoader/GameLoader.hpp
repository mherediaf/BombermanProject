#ifndef GAME_LOADER_HPP
#define GAME_LOADER_HPP

class GameLoader;

#include "../GameEntity/GameEntity.hpp"
#include "../Player/Player.hpp"
#include "../Field/Field.hpp"
#include "../Bomb/Bomb.hpp"
#include "../UnbreakableWall/UnbreakableWall.hpp"
#include "../BreakableWall/BreakableWall.hpp"
#include "../Flame/Flame.hpp"
#include "../Menu/Menu.hpp"

#include <string>


class GameLoader: public GameEntity
{
	public:
		GameLoader();
		~GameLoader();

		Field* load(std::string& nameFile, Menu* parent);
	
};

#endif