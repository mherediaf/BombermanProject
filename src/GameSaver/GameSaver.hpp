#ifndef GAME_SAVER_HPP
#define GAME_SAVER_HPP

class GameSaver;

#include "../Player/Player.hpp"
#include "../UnbreakableWall/UnbreakableWall.hpp"
#include "../BreakableWall/BreakableWall.hpp"
#include "../Bomb/Bomb.hpp"
#include "../Flame/Flame.hpp"
#include "../ConsoleCommand/ConsoleCommand.hpp"

#include <list>
#include <string>

class GameSaver: public GameEntity
{
	private:
		ConsoleCommand* console;
		
	public:
		GameSaver();
		~GameSaver();

		void save(std::string& nameFile, int numPlayers, Player** thePlayers, std::list< UnbreakableWall* >* theUnbreakableWalls,
			std::list< BreakableWall* >* theBreakableWalls, std::list< Bomb* >* theBombs, std::list< Flame* >* theFlames);
	
};

#endif