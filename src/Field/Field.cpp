#include "Field.hpp"
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

//#include "../SaveButton/SaveButton.hpp"

extern bool killWithEscape;

using namespace std;

Field::Field(int amountOfPlayers, _Window* screen, bool online, int idHostPlayer, Menu* parent, Connection* conn): SpritedEntity(){


	this->numberHostPlayer = idHostPlayer;

	this->_conn = conn;

	this->_parent = parent;
	this->_online = online;



	this->saver = new GameSaver();

	this->windowShown = screen;

	this->saveButton = new SpritedEntity();
	this->saveButton->loadBMP("../Images/SaveButton_2.bmp");

	cout << "created button" << endl;

	this->hopbar = new SpritedEntity();
	this->saveLayer = new SpritedEntity();
	this->hopbar->loadBMP("../Images/Toolbar.bmp");
	this->saveLayer->loadBMP("../Images/saveLayer.bmp");

	this->_amountOfPlayers = amountOfPlayers;

	this->thePlayers = NULL;
	this->theBombs = new list<Bomb*>();
	this->theBreakableWalls = new list<BreakableWall*>();
	this->theUnbreakableWalls = new list<UnbreakableWall*>();
	this->theFlames = new list<Flame*>();

	this->loadBMP("../Images/bg1.bmp");

	SDL_Rect toolbarPosition;

	toolbarPosition.x = 0;
	toolbarPosition.y = 0;

	SDL_Rect saveLayerPosition;

	saveLayerPosition.x = 40;
	saveLayerPosition.y = 4;

	this->windowShown->addLayer(this->hopbar->getSprite(), NULL, &toolbarPosition);
	this->windowShown->addLayer(this->saveButton->getSprite(), NULL, &saveLayerPosition);


	this->saveButtonHandlerSemaphore = new sem_t;
	this->saveLayerSemaphore = new sem_t;
	this->commandsListSem = new sem_t;

	sem_init(this->saveButtonHandlerSemaphore, 0, 0);
	sem_init(this->saveLayerSemaphore, 0, 0);
	sem_init(this->commandsListSem, 0, 0);

	this->running = true;
	this->started = false;

	this->runThread = new pthread_t;
	this->autosaveThread = new pthread_t;
	this->saveButtonHandlerThread = new pthread_t;
	this->recieverThread = new pthread_t;
	this->dispatcherThread = new pthread_t;
	this->positionThread = new pthread_t;

	this->gamePausedLock = new pthread_mutex_t;
	this->eventsLockMutex = new pthread_mutex_t;
	this->bombsMutex = new pthread_mutex_t;
	this->flamesMutex = new pthread_mutex_t;
	this->breakableWallsMutex = new pthread_mutex_t;
	this->startMutex = new pthread_mutex_t;
	this->commandsListMutex = new pthread_mutex_t;

	pthread_mutex_init(this->gamePausedLock, NULL);
	pthread_mutex_init(this->eventsLockMutex, NULL);
	pthread_mutex_init(this->bombsMutex, NULL);
	pthread_mutex_init(this->flamesMutex, NULL);
	pthread_mutex_init(this->breakableWallsMutex, NULL);
	pthread_mutex_init(this->startMutex, NULL);
	pthread_mutex_init(this->commandsListMutex, NULL);

	commandsList = new list< string* >;
	//	pthread_mutex_t* commandsListMutex;

	this->unblockEvents();

	this->setAutosaveEllapseTime(1);

	cout << "finished creating field" << endl;	

}

Field::~Field(){
	cout << "Trying to destroy the field" << endl;

	this->running = false;

	pthread_join(*(this->runThread), NULL);
	pthread_join(*(this->autosaveThread), NULL);

	sem_post(this->saveButtonHandlerSemaphore);
	pthread_join(*(this->saveButtonHandlerThread), NULL);

	if (this->isOnlineGame()){
		pthread_join(*(this->recieverThread), NULL);
		pthread_join(*(this->dispatcherThread), NULL);
		pthread_join(*(this->positionThread), NULL);
	}

	delete this->commandsList;


	if (this->isOnlineGame()){
		string* toSend = new string();
		stringstream ss;
		ss << "Disconnect " << this->getIDHostPlayer();
		*toSend = ss.str();
		cout << "Sending: [" << *toSend << "]" << endl;
		this->getConnection()->Send(toSend);
		delete toSend;
	}
//	delete this->windowShown;
	for (int i = 0; i < this->_amountOfPlayers; ++i)
	{
		delete this->thePlayers[i];
	}

	delete this->thePlayers;
	delete this->runThread;
	delete this->autosaveThread;
	delete this->saveButtonHandlerThread;
	delete this->recieverThread;
	delete this->dispatcherThread;
	delete this->positionThread;


	this->destroyUnbreakableWalls();
	this->destroyBreakableWalls();

	pthread_mutex_destroy(this->gamePausedLock);
	pthread_mutex_destroy(this->eventsLockMutex);
	pthread_mutex_destroy(this->bombsMutex);
	pthread_mutex_destroy(this->flamesMutex);
	pthread_mutex_destroy(this->breakableWallsMutex);
	pthread_mutex_destroy(this->startMutex);
	pthread_mutex_destroy(this->commandsListMutex);

	sem_destroy(this->saveButtonHandlerSemaphore);
	sem_destroy(this->saveLayerSemaphore);
	sem_destroy(this->commandsListSem);

	//SDL_FreeSurface(this->sprite);

	delete this->hopbar;

	cout << "Field destroyed correctly" << endl;
}

void Field::start(){
	pthread_mutex_lock(this->startMutex);
	this->started = true;
	pthread_mutex_unlock(this->startMutex);

	pthread_create(this->runThread, NULL, this->run, (void*) this);
	pthread_create(this->autosaveThread, NULL, this->autosaveHandler, (void*) this);
	pthread_create(this->saveButtonHandlerThread, NULL, this->saveButtonHandler, (void*) this);


	if (this->isOnlineGame()){
		pthread_create(this->recieverThread, NULL, this->recieverHandler, (void*) this);
		pthread_create(this->dispatcherThread, NULL, this->dispatcherHandler, (void*) this);
		pthread_create(this->positionThread, NULL, this->positionHandler, (void*) this);
	}


	cout << "OK" << endl << flush;
}

void Field::setPlayers(Player** _players, int num_players){
	if (this->thePlayers != NULL){
		for (int i = 0; i < this->_amountOfPlayers; ++i)
		{
			if (this->thePlayers[i] != NULL){
				delete this->thePlayers[i];
			}
		}
		delete this->thePlayers;
	}

	this->thePlayers = _players;
	this->_amountOfPlayers = num_players;
}

void Field::setUnbreakableWalls(std::list< UnbreakableWall* >* ubWalls){
	this->destroyUnbreakableWalls();

	this->theUnbreakableWalls = ubWalls;
}

void Field::setBreakableWalls(std::list< BreakableWall* >* bWalls){
	this->destroyBreakableWalls();

	this->theBreakableWalls = bWalls;
}

void Field::setBombs(std::list< Bomb* >* bombs){
	if (this->theBombs != NULL){
		for (list< Bomb* >::iterator it = this->theBombs->begin();
			it != this->theBombs->end(); it++)
		{
			if ((*it) != NULL){
				delete *it;
			}
		}

		delete this->theBombs;
	}

	this->theBombs = bombs;
}

void Field::setFlames(std::list< Flame* >* flames){
	if (this->theFlames != NULL){
		for (list< Flame* >::iterator it = this->theFlames->begin();
			it != this->theFlames->end(); it++)
		{
			if ((*it) != NULL){
				delete *it;
			}
		}

		delete this->theFlames;
	}

	this->theFlames = flames;
}

void Field::setNewGame(){
	Player** _players = new Player*[this->_amountOfPlayers];
	for (int i = 0; i < this->_amountOfPlayers; ++i)
	{
		bool online;
		if (this->isOnlineGame()){
			if (i == this->getIDHostPlayer()){
				online = false;
			} else{
				online = true;
			}
		} else{
			online = false;
		}
		_players[i] = new Player(i, online, this);
	}

	this->setPlayers(_players, this->_amountOfPlayers);

	this->generateUnbreakableWalls();
	this->generateBreakableWalls();

	this->destroyBombs();
	this->theBombs = new list< Bomb* >();

	this->destroyFlames();
	this->theFlames = new list< Flame* >();

	cout << "New game set correctly" << endl;
}


int Field::getAutosaveEllapseTime() const{
	return this->_autosaveEllapseTime;
}

void Field::setAutosaveEllapseTime(int time){
	this->_autosaveEllapseTime = time;
}

void* Field::autosaveHandler(void* params){
	Field* This = (Field*) params;

	string autosaveName;

	autosaveName = "autosave";

	while(This->running){
		sleep(This->getAutosaveEllapseTime());

		pthread_mutex_lock(This->gamePausedLock);
		This->saver->save(autosaveName, This->_amountOfPlayers, This->thePlayers, This->theUnbreakableWalls,
				This->theBreakableWalls, This->theBombs, This->theFlames);

	//	cout << "saved" << endl;

		pthread_mutex_unlock(This->gamePausedLock);
	}

	return NULL;
}

void* Field::saveButtonHandler(void* params){
	Field* This = (Field*) params;

	/////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////
	bool pressing = false;

	do
	{
		sem_wait(This->saveButtonHandlerSemaphore);
		
		if (This->_event->type == SDL_MOUSEBUTTONDOWN ){

			if( This->_event->button.button == SDL_BUTTON_LEFT ){
				if (!pressing){

					int x, y;
				//  if (!pressing){
				//  }
					x = This->_event->button.x;
					y = This->_event->button.y;

					if (x >= 40 && x < 40+70 && y>=4 && y<4+16){

					//	time_left = 100;
					//	pressing = true;

						This->blockEvents();

						killWithEscape = false;

						cout << "pressing save" << endl;
						This->deploySaveScreen();

					//	sleep(2);

						killWithEscape = true;
						This->unblockEvents();
					}
				}

				pressing = true;

			}
		}

		if ( This->_event->type == SDL_MOUSEBUTTONUP ){
			if( This->_event->button.button == SDL_BUTTON_LEFT ){
				pressing = false;

			}
		}

	} while (This->running);

	return NULL;
}

void* Field::run(void* params){
	unsigned long long tick = 0;
	Field* This = (Field*) params;
	do{
		//cout << "Tick: " << tick << endl;
		if (This->running){
		//  cout << "Still running" << endl;
			This->refresh();
			tick++;
			SDL_Delay(50);
		} else{
			cout << "Detected field no longer running" << endl;
		}
	} while(This->running);

	return NULL;
}

void Field::refresh(){
	this->generateLastLayer();
	this->draw();
}

void Field::generateLastLayer(){
	//this->toBePrinted = this->background;
	SDL_Rect backgroundPosition;
	backgroundPosition.x = 0;
	backgroundPosition.y = 20;

	if (!this->eventsAreBlocked()){



		this->windowShown->addLayer(this->sprite, NULL, &backgroundPosition);

		pthread_mutex_lock(this->gamePausedLock);

		// Print the unbreakable walls
		for (list<UnbreakableWall*>::iterator it = this->theUnbreakableWalls->begin();
				it != this->theUnbreakableWalls->end(); it++)
		{
			SDL_Surface* sprite_unbreakable_wall_i = (*it)->getSprite();
			int x = (*it)->getX();
			int y = (*it)->getY();
		//  cout << "(" << x << ", " << y << ")" << endl;

			SDL_Rect position;
			position.x = x;
			position.y = y + 20;

			this->windowShown->addLayer(sprite_unbreakable_wall_i, NULL, &position);
		}

		// Print the bombs

		pthread_mutex_lock(this->bombsMutex);

		list<Bomb*>* auxBombs = new list<Bomb*>();

		list<Bomb*>::iterator itBombs;
		itBombs = this->theBombs->begin();
		//cout << "cant bombs: " << this->theBombs->size() << endl;
		while ( itBombs != this->theBombs->end() )
		{
			SDL_Surface* sprite_bomb_i = (*itBombs)->getSprite();
			int x = (*itBombs)->getX();
			int y = (*itBombs)->getY();

			SDL_Rect position;
			position.x = x;
			position.y = y + 20;

			this->windowShown->addLayer(sprite_bomb_i, NULL, &position);

			if (!this->eventsAreBlocked()){

				if ((*itBombs)->mustBeDestroyed()){
					//cout << "Destroying bomb" << endl;
					int power = (*itBombs)->getPower();

					//cout << "bomb has to explode" << endl;
					this->bombExplode(x, y, power, *itBombs);


					delete (*itBombs);
					//this->theBombs->erase(itBombs);
				} else{
					(*itBombs)->tick();
					//cout << "Made a tick" << endl;
					auxBombs->push_back(*itBombs);
				}
			}
			itBombs++;
		}

		delete this->theBombs;
		this->theBombs = auxBombs;

		pthread_mutex_unlock(this->bombsMutex);
		// Print the breakable walls

		//cout << "Amount of breakable walls: " << this->theBreakableWalls->size() << endl;
		//list<BreakableWall*>* auxBWalls;
		//auxBWalls = new list<BreakableWall*>();

		pthread_mutex_lock(this->breakableWallsMutex);

		for (list<BreakableWall*>::iterator it = this->theBreakableWalls->begin();
				it != this->theBreakableWalls->end(); it++)
		{

			SDL_Surface* sprite_breakable_wall_i = (*it)->getSprite();
			int x = (*it)->getX();
			int y = (*it)->getY();
		//  cout << "(" << x << ", " << y << ")" << endl;

			SDL_Rect position;
			position.x = x;
			position.y = y + 20;

			this->windowShown->addLayer(sprite_breakable_wall_i, NULL, &position);

		}

		// Print the players
		for (int i = 0; i < this->_amountOfPlayers; ++i)
		{
			//cout << "Getting sprite from player " << i+1 << endl;
			SDL_Surface* sprite_player_i = this->thePlayers[i]->getSprite();
			int x = this->thePlayers[i]->getX();
			int y = this->thePlayers[i]->getY();

			//cout << "Position player " << i+1 << ": (" << x << ", " << y << ")" << endl;

			SDL_Rect position;
			position.x = x;
			position.y = y + 20;

			this->windowShown->addLayer(sprite_player_i, NULL, &position);
			//cout << "Added layer of player " << i+1 << " sprite" << endl;

		}

		pthread_mutex_unlock(this->breakableWallsMutex);
		// Print the flames

		pthread_mutex_lock(this->flamesMutex);

		list<Flame*>::iterator it;
		it = this->theFlames->begin();

		list<Flame*> * auxFlames = new list<Flame*>();

		while ( it != this->theFlames->end() )
		{
			SDL_Surface* sprite_flame_i = (*it)->getSprite();
			int x = (*it)->getX();
			int y = (*it)->getY();

			//cout << "Position player " << i+1 << ": (" << x << ", " << y << ")" << endl;

			SDL_Rect position;
			position.x = x;
			position.y = y + 20;

			this->windowShown->addLayer(sprite_flame_i, NULL, &position);

			this->calculate_flame_contact_with_bombs(*it);
			this->calculate_flame_contact_with_players(*it);

			if (!this->eventsAreBlocked()){


				if ((*it)->mustBeDestroyed()){
					//cout << "Destroying bomb" << endl;
					delete (*it);
					//this->theBombs->erase(it);
				} else{
					(*it)->tick();
					//cout << "Made a tick" << endl;
					auxFlames->push_back(*it);
				}

			}

			it++;
		}

		delete this->theFlames;
		this->theFlames = auxFlames;
		pthread_mutex_unlock(this->flamesMutex);

		pthread_mutex_unlock(this->gamePausedLock);
	}

}

void Field::draw(){
	this->windowShown->show();
}

void Field::keyPressed(bool* keys, SDL_Event* event){
	this->_keys = keys;
	pthread_mutex_lock(this->startMutex);

	bool quit = (keys[SDLK_ESCAPE] || event->type == SDL_QUIT);
	
	if (this->started){
		pthread_mutex_lock(this->eventsLockMutex);
		if (!this->eventsAreBlocked()){

			this->_event = event;
			sem_post(this->saveButtonHandlerSemaphore);

			for (int i = 0; i < this->_amountOfPlayers; ++i)
			{
				this->thePlayers[i]->keyPressed(keys);
			}

			if (quit){
				this->_parent->destroyField();
			}
		} else{
			sem_post(this->saveLayerSemaphore);
		}
	}
	pthread_mutex_unlock(this->eventsLockMutex);

	pthread_mutex_unlock(this->startMutex);
}

_Window* Field::getScreen(){
	return this->windowShown;
}

void Field::generateBreakableWalls(){
	BreakableWall* thisBWall;
	list< BreakableWall* >* listBWalls = new list< BreakableWall* >();

	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			thisBWall = new BreakableWall();
			
			thisBWall->setX(i*30);
			thisBWall->setY(j*30);

			if (i == 1 && j == 1){

			} else if (i == 2 && j == 1){
				delete thisBWall;
			} else if (i == 1 && j == 2){
				delete thisBWall;
			} else if (i == 8 && j == 8){
				delete thisBWall;
			} else if (i == 7 && j == 8){
				delete thisBWall;
			} else if (i == 8 && j == 7){
				delete thisBWall;
			} else if (this->canTheEntityBeThere(thisBWall)){
				listBWalls->push_back(thisBWall);
			} else{
				delete thisBWall;
			}
		}
	}

	this->setBreakableWalls(listBWalls);
}

void Field::generateUnbreakableWalls(){
	UnbreakableWall* thisUWall;
	list< UnbreakableWall* >* listUWalls = new list< UnbreakableWall* >();

	for (int i = 0; i < 10; ++i)
	{
		thisUWall = new UnbreakableWall();

		thisUWall->setX(i*30);
		thisUWall->setY(0);

		listUWalls->push_back(thisUWall);
	}

	for (int i = 0; i < 10; ++i)
	{
		thisUWall = new UnbreakableWall();

		thisUWall->setX(i*30);
		thisUWall->setY(300-30);

		listUWalls->push_back(thisUWall);
	}

	for (int j = 1; j < 9; ++j)
	{
		thisUWall = new UnbreakableWall();

		thisUWall->setX(0);
		thisUWall->setY(j*30);

		listUWalls->push_back(thisUWall);
	}
	for (int j = 1; j < 9; ++j)
	{
		thisUWall = new UnbreakableWall();

		thisUWall->setX(300-30);
		thisUWall->setY(j*30);

		listUWalls->push_back(thisUWall);
	}

	thisUWall = new UnbreakableWall();
	thisUWall->setX(120);
	thisUWall->setY(30);

	listUWalls->push_back(thisUWall);

	thisUWall = new UnbreakableWall();
	thisUWall->setX(150);
	thisUWall->setY(30);

	listUWalls->push_back(thisUWall);

	thisUWall = new UnbreakableWall();
	thisUWall->setX(120);
	thisUWall->setY(240);

	listUWalls->push_back(thisUWall);

	thisUWall = new UnbreakableWall();
	thisUWall->setX(150);
	thisUWall->setY(240);

	listUWalls->push_back(thisUWall);

////////////////////////    
	thisUWall = new UnbreakableWall();
	thisUWall->setX(30);
	thisUWall->setY(120);

	listUWalls->push_back(thisUWall);

	thisUWall = new UnbreakableWall();
	thisUWall->setX(30);
	thisUWall->setY(150);

	listUWalls->push_back(thisUWall);

	thisUWall = new UnbreakableWall();
	thisUWall->setX(240);
	thisUWall->setY(120);

	listUWalls->push_back(thisUWall);

	thisUWall = new UnbreakableWall();
	thisUWall->setX(240);
	thisUWall->setY(150);

	listUWalls->push_back(thisUWall);

	thisUWall = new UnbreakableWall();
	thisUWall->setX(60);
	thisUWall->setY(60);

	listUWalls->push_back(thisUWall);

	thisUWall = new UnbreakableWall();
	thisUWall->setX(60);
	thisUWall->setY(210);

	listUWalls->push_back(thisUWall);

	thisUWall = new UnbreakableWall();
	thisUWall->setX(210);
	thisUWall->setY(210);

	listUWalls->push_back(thisUWall);

	thisUWall = new UnbreakableWall();
	thisUWall->setX(210);
	thisUWall->setY(60);

	listUWalls->push_back(thisUWall);

	this->setUnbreakableWalls(listUWalls);
}

void Field::destroyUnbreakableWalls(){

	if (this->theUnbreakableWalls != NULL){
		for (list<UnbreakableWall*>::iterator it = this->theUnbreakableWalls->begin();
				it != this->theUnbreakableWalls->end(); it++)
		{
			if ((*it) != NULL){
				delete *it;
			}
		}

		delete this->theUnbreakableWalls;
	}
}

void Field::destroyBreakableWalls(){

	if (this->theBreakableWalls != NULL){
		for (list<BreakableWall*>::iterator it = this->theBreakableWalls->begin();
				it != this->theBreakableWalls->end(); it++)
		{
			if ((*it) != NULL){
				delete *it;
			}
		}

		delete this->theBreakableWalls;
	}
}

void Field::destroyBombs(){
	if (this->theBombs != NULL){
		for (list<Bomb*>::iterator it = this->theBombs->begin();
				it != this->theBombs->end(); it++)
		{
			if ((*it) != NULL){
				delete *it;
			}
		}

		delete this->theBombs;
	}
}

void Field::destroyFlames(){
	if (this->theFlames != NULL){
		for (list<Flame*>::iterator it = this->theFlames->begin();
				it != this->theFlames->end(); it++)
		{
			if ((*it) != NULL){
				delete *it;
			}
		}

		delete this->theFlames;
	}
}


bool Field::canTheEntityBeThere(GameObject* entity){
	bool res = true;

	for (list<UnbreakableWall*>::iterator it = this->theUnbreakableWalls->begin();
			res && it != this->theUnbreakableWalls->end(); it++)
	{
		if (!(*it)->canTheMovingEntityBeHere(entity) ) {
			res = false;
		}
	}

	for (list<BreakableWall*>::iterator it = this->theBreakableWalls->begin();
			res && it != this->theBreakableWalls->end(); it++)
	{
		if (!(*it)->canTheMovingEntityBeHere(entity) ) {
			res = false;
		}
	}

	for (list<Bomb*>::iterator it = this->theBombs->begin();
			res && it != this->theBombs->end(); it++)
	{
		if (!(*it)->canTheMovingEntityBeHere(entity)){
			res = false;
		}
	}

	return res;
}

bool Field::dropBomb(Player* from, int x, int y, int ticks, int power){
	int pos_x, pos_y;
	pos_x = (x+15) / 30;
	pos_x *= 30;
	
	pos_y = (y+15) / 30;
	pos_y *= 30;

	bool res;

	Bomb* newBomb = new Bomb(ticks, pos_x, pos_y, power, from, this->getScreen());

	if (this->canTheEntityBeThere(newBomb) ) {
		res = true;

		pthread_mutex_lock(this->bombsMutex);

		newBomb->activate();
		this->theBombs->push_back(newBomb);

		if (this->isOnlineGame()){
			string* toSend = new string();
			stringstream ss;
			ss << "Bomb " << this->getIDHostPlayer() << " " << power << " " << ticks << " " << pos_x << " " << pos_y;
			*toSend = ss.str();
			cout << "Sending: [" << *toSend << "]" << endl;
			this->getConnection()->Send(toSend);
			delete toSend;
		}

		pthread_mutex_unlock(this->bombsMutex);
	} else{
		delete newBomb;
		res = false;
	}

	return res;
}

void Field::bombExplode(int x, int y, int power, Bomb* from){
	this->bombExplodeDirection(x, y, power, from, 0);
	this->bombExplodeDirection(x, y, power, from, 1);
	this->bombExplodeDirection(x, y, power, from, 2);
	this->bombExplodeDirection(x, y, power, from, 3);
}

void Field::bombExplodeDirection(int x, int y, int power, Bomb *from, int direction){
	bool tryAlsoNextPosition;
	bool canBePlaced;
	Flame* newFlame;

	list<BreakableWall*>* auxBreakableWalls;
	list<BreakableWall*>::iterator itBWalls;

	tryAlsoNextPosition = true;
	canBePlaced = true;


	for (int i = 0; tryAlsoNextPosition && i <= power; ++i)
	{
		// Y --
		int newX, newY;
		if (direction == 0){ // Up
			newX = x;
			newY = y - i*30;
		} else if (direction == 1){ // Down
			newX = x;
			newY = y + i*30;
		} else if (direction == 2){ // Left
			newX = x - i*30;
			newY = y;
		} else { // Right
			newX = x + i*30;
			newY = y;
		}

		newFlame = new Flame(newX, newY, this->getScreen(), from);

		for (list<UnbreakableWall*>::iterator it = this->theUnbreakableWalls->begin();
			canBePlaced && it != this->theUnbreakableWalls->end(); it++)
		{
			if (!(*it)->canTheMovingEntityBeHere(newFlame) ) {
				canBePlaced = false;
				tryAlsoNextPosition = false;
			}
		}

		auxBreakableWalls = new list<BreakableWall*>();
		itBWalls = this->theBreakableWalls->begin();

		while ( itBWalls != this->theBreakableWalls->end() )
		{
			if (!(*itBWalls)->canTheMovingEntityBeHere(newFlame) ) {
				tryAlsoNextPosition = false;
			} else{
				auxBreakableWalls->push_back(*itBWalls);
			}
			itBWalls++;
		}

		delete this->theBreakableWalls;
		this->theBreakableWalls = auxBreakableWalls;

		if (canBePlaced){
			pthread_mutex_lock(this->flamesMutex);
			this->theFlames->push_back(newFlame);
			pthread_mutex_unlock(this->flamesMutex);
		} else{
			delete newFlame;
		}

		
		for (list<Bomb*>::iterator itBombs = this->theBombs->begin();
			tryAlsoNextPosition && itBombs != this->theBombs->end(); itBombs++)
		{
			if (from != (*itBombs) && !(*itBombs)->canTheMovingEntityBeHere(newFlame)){
				tryAlsoNextPosition = false;
			}
		}

	}
}

void Field::calculate_flame_contact_with_bombs(Flame* thisFlame){
	Bomb* bomb_from;
	bomb_from = thisFlame->getBombFrom();
	for (list<Bomb*>::iterator itBombs = this->theBombs->begin();
		itBombs != this->theBombs->end(); itBombs++)
	{
		if (bomb_from != (*itBombs) && !(*itBombs)->canTheMovingEntityBeHere(thisFlame)){
			//cout << "Fire touching bomb" << endl;
			(*itBombs)->explode();
			//cout << "bomb about to explode" << endl;
		}
	}
}

void Field::calculate_flame_contact_with_players(Flame* thisFlame){
	for (int i = 0; i < this->_amountOfPlayers; ++i)
	{
		if (thisFlame->aPlayerIsTouching(this->thePlayers[i])){
			thePlayers[i]->die();
		}
	}
}

void Field::blockEvents(){
	pthread_mutex_lock(this->eventsLockMutex);

	this->eventsLock = true;
	
	pthread_mutex_unlock(this->eventsLockMutex);
}

void Field::unblockEvents(){
	pthread_mutex_lock(this->eventsLockMutex);

	this->eventsLock = false;

	pthread_mutex_unlock(this->eventsLockMutex);
}

bool Field::eventsAreBlocked(){
//	pthread_mutex_lock(this->eventsLockMutex);

	bool res;
	res = this->eventsLock;
	
//	pthread_mutex_unlock(this->eventsLockMutex);

	return res;
}

bool Field::isOnlineGame(){
	return this->_online;
}

void Field::deploySaveScreen(){

//	if (TTF_Init() != 0)
//	{
//		cerr << "TTF_Init() Failed: " << TTF_GetError() << endl;
//		return;
//	}

	char name[10];
	for (int i = 1; i < 10; ++i)
	{
		name[i] = '\0';
	}
	name[0] = ' ';
	int pointerPosition = 1;

	TTF_Font *font;
	font = TTF_OpenFont("../Fonts/FreeSans.ttf", 16);

	if (font == NULL)
	{
		cerr << "TTF_OpenFont() Failed: " << TTF_GetError() << endl;
		TTF_Quit();
		return;
	}

	SDL_Surface *text;
	SDL_Color text_color = {0, 0, 0};



	SDL_Rect backgroundPosition;
	backgroundPosition.x = 0;
	backgroundPosition.y = 20;

	SDL_Rect fontPosition;
	fontPosition.x = 100;
	fontPosition.y = 100+20;

	bool pressedAny = false;
	int lastPressed;

//	int counter = 0;

//	bool pressing = false;

	while(true){
		this->windowShown->addLayer(this->saveLayer->getSprite(), NULL, &backgroundPosition);
		text = TTF_RenderText_Solid(font, &name[0], text_color);
		if (text == NULL)
		{
			cerr << "TTF_RenderText_Solid() Failed: " << TTF_GetError() << endl;
			TTF_Quit();
			return;
		}
		this->windowShown->addLayer(text, NULL, &fontPosition);

	//	if (SDL_BlitSurface(this->windowShown->getSprite(), NULL, text, &fontPosition) != 0){
	//		cout << "Error displaying text" << endl;
	//		cout << TTF_GetError() << endl;
	//	}
		this->windowShown->show();

	//	cout << "Save detected key" << endl;
	//	cout << counter << endl;
	//	counter++;
		bool found;
		sem_wait(this->saveLayerSemaphore);

		if (this->_event->type == SDL_KEYDOWN){

	//		if (!pressing){

				if (this->_keys[SDLK_ESCAPE] || this->_event->type == SDL_QUIT){
					break;
				}

				if (this->_keys[SDLK_RETURN]){
					if (pointerPosition > 1){

						char aux[9];
						for (int i = 0; i < 9; ++i)
						{
							aux[i] = name[i+1];
						}
						string saveName = aux;
						this->saver->save(saveName, this->_amountOfPlayers, this->thePlayers, this->theUnbreakableWalls,
						this->theBreakableWalls, this->theBombs, this->theFlames);

						break;
					}
				}

				if (!pressedAny || !this->_keys[lastPressed]){

					if (this->_keys[SDLK_BACKSPACE]){
						if (pointerPosition > 1){
							pointerPosition--;

							name[pointerPosition] = '\0';

							cout << "Backspace" << endl;

							pressedAny = false;
						//	lastPressed = SDLK_BACKSPACE;
						}
					}

					found = false;

					for (int i = SDLK_0; !found && i <= SDLK_9; ++i)
					{
						if (this->_keys[i]){
							if (pointerPosition < 9){
								name[pointerPosition] = '0' - SDLK_0 + i;
								pointerPosition++;
							//	cout << "0" << endl;
								pressedAny = true;
								lastPressed = i;

								found = true;
							}
						}
					}

					for (int i = SDLK_a; !found && i <= SDLK_z; ++i)
					{
						if (this->_keys[i]){
							if (pointerPosition < 9){
								name[pointerPosition] = 'a' - SDLK_a + i;
								pointerPosition++;
							//	cout << "0" << endl;
								pressedAny = true;
								lastPressed = i;

								found = true;
							}
						}
					}
					
					

				}
			}
	//		pressing = true;
	//	} else if (this->_event->type == SDL_KEYDOWN){
	//		pressing = false;
	//	}

	}

//	TTF_Quit();
}

int Field::getIDHostPlayer() const{
	return this->numberHostPlayer;
}

void* Field::recieverHandler(void* params){
	Field* This = (Field*) params;

	string* recieved;
	while(This->running){
		recieved = This->getConnection()->Recieve();
	//	pthread_mutex_lock(This->commandsListMutex);
		This->commandsList->push_back(recieved);
		sem_post(This->commandsListSem);
	//	pthread_mutex_unlock(This->commandsListMutex);
	}

	return NULL;
}

void* Field::dispatcherHandler(void* params){
	Field* This = (Field*) params;

	string* first;
	while(This->running){
		sem_wait(This->commandsListSem);

		if (!This->running){
			break;
		}

		if (This->commandsList->size() > 0){

			first = This->commandsList->front();
			This->commandsList->pop_front();

	//		pthread_mutex_unlock(This->commandsListMutex);

			This->makeAction(first);
			
		} else{
	//		pthread_mutex_unlock(This->commandsListMutex);
		}
		
	}

	return NULL;
}

Connection* Field::getConnection(){
	return this->_conn;
}

void Field::makeAction(string* action){
	stringstream ss;
	ss << *action;

	string command;
	ss >> command;

	if (command.compare("Position") == 0){
		// Updating position from other player
		int id;
		ss >> id;

		int x;
		int y;

		ss >> x;
		ss >> y;


		this->thePlayers[id]->setX(x);
		this->thePlayers[id]->setY(y);

		// Updated
	} else if (command.compare("Bomb") == 0) {
		// Other player placed a bomb

		int id;

		int power;
		int ticks;

		int x;
		int y;

		ss >> id;
		ss >> power;
		ss >> ticks;
		ss >> x;
		ss >> y;

		this->dropBomb(this->thePlayers[id], x, y, ticks, power);


	} else if (command.compare("Disconnect") == 0){
		int id;
		ss >> id;

		this->thePlayers[id]->die();
		this->running = false;
		// Other player disconnected
	} else if (command.compare("Die") == 0){
		// A player died
		int id;
		ss >> id;

		this->thePlayers[id]->die();
	} else{
		cout << "Command unkwown" << endl;
	}
}

void* Field::positionHandler(void* params){
	Field* This = (Field*) params;

	int i = This->getIDHostPlayer();

	while (This->running){
		sleep(1);
		string* toSend = new string();
		stringstream ss;
		int x, y;
		x = This->thePlayers[i]->getX();
		y = This->thePlayers[i]->getY();
		ss << "Position " << i << " " << x << " " << y;
		*toSend = ss.str();
		cout << "Sending: [" << *toSend << "]" << endl;
		This->getConnection()->Send(toSend);
		delete toSend;

	}

	return NULL;
}