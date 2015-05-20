#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <iostream>

#include <pthread.h>
#include <sys/types.h>

#include "Field/Field.hpp"
#include "Player/Player.hpp"

#include "Window/Window.hpp"
#include "Menu/Menu.hpp"

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <X11/Xlib.h> 

using namespace std;

void* keyPressing(void* args);
void* create (void* args);


//Player* player1;
//Field* field;
_Window* screen;
Menu* menu;

bool gameRunning;
sem_t* field_create;
sem_t* key_starting_detecting;
sem_t* game_finish;

bool keysHeld[323] = {false}; // everything will be initialized to false
bool killWithEscape;

int main(int argc, char* argv[])
{

	XInitThreads();


	field_create = new sem_t;
	key_starting_detecting = new sem_t;
	game_finish = new sem_t;

	sem_init(field_create, 0, 0);
	sem_init(key_starting_detecting, 0, 0);
	sem_init(game_finish, 0, 0);

	SDL_Init(SDL_INIT_EVERYTHING);

	TTF_Init();

//	cout << "llego" << endl;

	killWithEscape = true;

	screen = new _Window(300, 320);
	menu = new Menu(screen);

	gameRunning = true;

	pthread_t keyPressingThread;
	pthread_t createThread;

	pthread_create(&keyPressingThread, NULL, keyPressing, NULL);
	pthread_create(&createThread, NULL, create, NULL);

	sem_wait(key_starting_detecting);
	pthread_join(createThread, NULL);
	
	sem_wait(game_finish);
	pthread_join(keyPressingThread, NULL);

	sem_destroy(field_create);
	sem_destroy(key_starting_detecting);
	sem_destroy(game_finish);

//	delete field;

	delete menu;
	delete screen;

	TTF_Quit();
	SDL_Quit();

	return 0;
}



void* keyPressing(void* args){
	//Uint8* keys;
	SDL_Event event;
	sem_wait(field_create);
	cout << "Starting to detect key pressing" << endl;
	sem_post(key_starting_detecting);
	do
	{
		if (SDL_PollEvent(&event))
	//	if (SDL_WaitEvent(&event))
		{

			if (event.type == SDL_KEYDOWN)
			{
				keysHeld[event.key.keysym.sym] = true;
			} 
			if (event.type == SDL_KEYUP)
			{
				keysHeld[event.key.keysym.sym] = false;
			}

			if (keysHeld[SDLK_ESCAPE] || event.type == SDL_QUIT){
				if (menu->finishWithEscape()){
					cout << "Finishing all" << endl;
					gameRunning = false;
					break;
				} else{
					cout << "NOT finishing all" << endl;
				}
			}
		}

		menu->keyPressed(&keysHeld[0], &event);

		SDL_Delay(3);
		//player2->keyPressed(c);
	} while (gameRunning);

	sem_post(game_finish);

	return NULL;
}

void* create (void* args){
	
//	cout << "trying" << endl;
//	field = new Field(2, screen, false);
//	cout << "Field created" << endl;
	sem_post(field_create);

	return NULL;
}