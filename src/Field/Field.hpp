#ifndef FIELD_HPP
#define FIELD_HPP

class Field;

#include "../GameEntity/GameEntity.hpp"
#include "../Player/Player.hpp"
#include "../Window/Window.hpp"
#include "../UnbreakableWall/UnbreakableWall.hpp"
#include "../BreakableWall/BreakableWall.hpp"
#include "../Bomb/Bomb.hpp" 
#include "../GameObject/GameObject.hpp"
#include "../Flame/Flame.hpp"
#include "../GameSaver/GameSaver.hpp"
#include "../Menu/Menu.hpp"
#include "../Connection/Connection.hpp"

#include <pthread.h>
#include <sys/types.h>
#include <semaphore.h>

#include <list>
#include <vector>
#include <sstream>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#define SAVER_BUTTON_ID 0

class Field: public SpritedEntity{
	private:
		Menu* _parent;
		GameSaver* saver;
		_Window* windowShown;

		SpritedEntity* hopbar;
		SpritedEntity* saveLayer;
		SpritedEntity* saveButton;

		Player** thePlayers;
		//Bombs
		std::list<Bomb*>* theBombs;
		//Walls
		std::list<BreakableWall*>* theBreakableWalls;
		std::list<UnbreakableWall*>* theUnbreakableWalls;

		std::list<Flame*> * theFlames;

		pthread_t* runThread;
		pthread_t* autosaveThread;

		pthread_t* saveButtonHandlerThread;

		pthread_t* recieverThread;
		pthread_t* dispatcherThread;
		pthread_t* positionThread;

		std::list< std::string* >* commandsList;
		pthread_mutex_t* commandsListMutex;
		

		pthread_mutex_t* gamePausedLock;
		pthread_mutex_t* eventsLockMutex;

		pthread_mutex_t* bombsMutex;
		pthread_mutex_t* flamesMutex;
		pthread_mutex_t* breakableWallsMutex;
		pthread_mutex_t* startMutex;

		bool eventsLock;

		sem_t* saveButtonHandlerSemaphore;
		sem_t* saveLayerSemaphore;
		sem_t* commandsListSem;
	//	bool gamePaused;

		static void* run(void* params);
		static void* autosaveHandler(void* params);
		static void* saveButtonHandler(void* params);

		static void* recieverHandler(void* params);
		static void* dispatcherHandler(void* params);
		static void* positionHandler(void* params);

		void refresh();
		void generateLastLayer();
		void draw();

		void generateBreakableWalls();
		void generateUnbreakableWalls();

		void destroyBreakableWalls();
		void destroyUnbreakableWalls();
		void destroyBombs();
		void destroyFlames();

		//void addFlame();

		void bombExplode(int x, int y, int power, Bomb* from);
		void bombExplodeDirection(int x, int y, int power, Bomb* from, int direction);

		void calculate_flame_contact_with_bombs(Flame* thisFlame);
		void calculate_flame_contact_with_players(Flame* thisFlame);

		void handleButtonPress(int button_id);

		int id;
		int _amountOfPlayers;
		int _amountOfButtons;

		int _autosaveEllapseTime;

		//SDL_Surface* sprite;

		bool running;
		bool started;
		bool _online;

		bool isOnlineGame();

		void deploySaveScreen();

		SDL_Event* _event;
		bool* _keys;

		int numberHostPlayer;

		int getIDHostPlayer() const;

		Connection* _conn;

		Connection* getConnection();
		void makeAction(std::string* action);

	public:
		Field(int amountOfPlayers, _Window* screen, bool online, int idHostPlayer, Menu* parent, Connection* conn);
		~Field();

		void keyPressed(bool* keys, SDL_Event* event);
		_Window* getScreen();

		bool canTheEntityBeThere(GameObject* entity);
		bool dropBomb(Player* from, int x, int y, int ticks, int power);

		int getAutosaveEllapseTime() const;
		void setAutosaveEllapseTime(int time);

		void blockEvents();
		void unblockEvents();

		bool eventsAreBlocked();

		void setPlayers(Player** _players, int amount);
		void setUnbreakableWalls(std::list< UnbreakableWall* >* ubWalls);
		void setBreakableWalls(std::list< BreakableWall* >* bWalls);
		void setBombs(std::list< Bomb* >* bombs);
		void setFlames(std::list< Flame* >* flames);

		void setNewGame();

		void start();
};

#endif