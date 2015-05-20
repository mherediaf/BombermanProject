#ifndef MENU_HPP
#define MENU_HPP

class Menu;

#include "../SpritedEntity/SpritedEntity.hpp"
#include "../Field/Field.hpp"
#include "../Window/Window.hpp"
#include "../TcpServer/TcpServer.hpp"
#include "../TcpClient/TcpClient.hpp"

#include <pthread.h>
#include <sys/types.h>
#include <SDL/SDL.h>

#define MENU_SCREEN 0
#define PLAYING_GAME 1
#define LOADING_GAME 2
#define CHOOSING_SERVER_OR_CLIENT 3
#define AWAITING_CLIENT_CONNECTION 4
#define CHOOSING_SERVER_IP 5

#include <SDL/SDL_ttf.h>

class Menu: public SpritedEntity
{
	private:
		SpritedEntity* continueButton;
		SpritedEntity* loadButton;
		SpritedEntity* PvPButton;
		SpritedEntity* onlineButton;
		SpritedEntity* serverButton;
		SpritedEntity* clientButton;
		SpritedEntity* loadSprite;
		SpritedEntity* IPSprite;

		TTF_Font *loadFont;
		SDL_Surface* loadText;
		SDL_Color text_color;

		pthread_t* continueButtonThread;
		pthread_t* loadButtonThread;
		pthread_t* PvPButtonThread;
		pthread_t* onlineButtonThread;
		pthread_t* serverButtonThread;
		pthread_t* clientButtonThread;

		SDL_Rect continuePosition;
		SDL_Rect loadPosition;
		SDL_Rect PVPPosition;
		SDL_Rect onlinePosition;

		SDL_Rect serverPosition;
		SDL_Rect clientPosition;

		sem_t* continueButtonSem;
		sem_t* PVPButtonSem;
		sem_t* onlineButtonSem;
		sem_t* loadButtonSem;
		sem_t* loadSpriteSem;
		sem_t* ipSpriteSem;

		sem_t* serverButtonSem;
		sem_t* clientButtonSem;

		static void* continueButtonHandler(void* args);
		static void* loadButtonHandler(void* args);
		static void* PvPButtonHandler(void* args);
		static void* onlineButtonHandler(void* args);

		static void* serverButtonHandler(void* args);
		static void* clientButtonHandler(void* args);

		pthread_mutex_t* gameStateMutex;
		pthread_mutex_t* eventsBlockMutex;

		_Window* _screen;
		Field* game;

		int gameState;

		bool running;
		bool eventsBlocked;

		int mouse_x;
		int mouse_y;

		void blockEvents();
		void unblockEvents();

		bool eventsAreBlocked();

		bool pressing;

		void showMenu();
		void showServerOrClient();
		void showLoadingScreen();

		void showAwaitingConnection();
		void showIPConnection();

		void deploySaveScreen();

		bool lastPressedQuit;

		SDL_Event* _event;
		bool* _keys;

	public:
		Menu(_Window* screen);
		~Menu();

		_Window* getScreen();

		void keyPressed(bool* keys, SDL_Event* event);

		bool finishWithEscape();

		void destroyField();
	
};
#endif