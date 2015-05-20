#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <SDL/SDL.h>

class Player;
#include "../MovingEntity/MovingEntity.hpp"
#include "../Field/Field.hpp"


struct this_and_char{
	Player* This;
	int c;
};

class Player: public MovingEntity{

	private:

		static void* keyController(void* params);
		static void* upKey(void* params);
		static void* downKey(void* params);
		static void* leftKey(void* params);
		static void* rightKey(void* params);
		static void* place_bomb(void* params);

		void loadDefaultPosition();
		void loadDefaultKeys();


		sem_t* upKeySemaphore;
		sem_t* downKeySemaphore;
		sem_t* leftKeySemaphore;
		sem_t* rightKeySemaphore;
		sem_t* placeBombSemaphore;
		sem_t* keyControllerSemaphore;

		pthread_t* upKeyThread;
		pthread_t* downKeyThread;
		pthread_t* leftKeyThread;
		pthread_t* rightKeyThread;

		pthread_t* placeBombThread;

		pthread_t* keyControllerThread;

		int UP_KEY;
		int DOWN_KEY;
		int RIGHT_KEY;
		int LEFT_KEY;
		int BOMB_KEY;

		int id;

		bool* lastKeyPressed;

		bool running;

		Field* belongsTo;

		int ticksForBombs;
		int _power;

		int maxCantBombs;

		int cantBombsPlaced;
		void placedOneBomb();

		bool _dead;
		bool _online;

		bool isOnline();

	public:
		Player(int id, bool online, Field* belongsTo);
		~Player();

		void keyPressed(bool* keys);
		int getID() const;

		int getTicksForBombs() const;
		int getPower() const;
		int getMaxCantBombs() const;

		void setTicksForBombs(int ticks);
		void setPower(int power);
		void setMaxCantBombs(int cantBombs);

		void oneBombExploded();

		int getCantBombsPlaced() const;
		void setCantBombsPlaced(int _cant);

		void die();

		bool isDead() const;

		int getUpKey() const;
		int getDownKey() const;
		int getRightKey() const;
		int getLeftKey() const;
		int getBombKey() const;
};

#endif