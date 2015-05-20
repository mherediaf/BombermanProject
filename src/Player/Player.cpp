#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <iostream>
#include <string>

#include "Player.hpp"
#include "../Constants/constants_players_keys.hpp"
#include "../Constants/constants.hpp"
#include "../Window/Window.hpp"

using namespace std;

Player::Player(int id, bool online, Field* belongsTo): MovingEntity(){

	this->_online = online;
	this->id = id;

	string path;
	path = "../Images/Player/player.";
	path +=  (char)(this->id + 48) + 1;
	path += ".bmp";

	this->loadBMP(path);

	cout << "image loaded" << endl;

	this->running = true;

	this->_dead = false;

	this->setTicksForBombs(30);
	this->setPower(2);
	this->setMaxCantBombs(2);
	this->cantBombsPlaced = 0;

	this->belongsTo = belongsTo;

	cout << "before screen" << endl << flush;
	Uint32 colorkey = SDL_MapRGB(this->belongsTo->getScreen()->getFormat(), 0xFF, 0, 0xFF);
	cout << "after screen" << endl << flush;

	SDL_SetColorKey(this->getSprite(), SDL_SRCCOLORKEY, colorkey);

	this->setSpeed (1.0);

	this->loadDefaultPosition();

	this->loadDefaultKeys();

	cout << "Keys loades" << endl;

	upKeySemaphore = new sem_t;
	downKeySemaphore = new sem_t;
	leftKeySemaphore = new sem_t;
	rightKeySemaphore = new sem_t;
	placeBombSemaphore = new sem_t;
	keyControllerSemaphore = new sem_t;

	sem_init(upKeySemaphore, 0, 0);
	sem_init(downKeySemaphore, 0, 0);
	sem_init(leftKeySemaphore, 0, 0);
	sem_init(rightKeySemaphore, 0, 0);
	sem_init(placeBombSemaphore, 0, 0);
	sem_init(keyControllerSemaphore, 0, 0);

	upKeyThread = new pthread_t;
	downKeyThread = new pthread_t;
	leftKeyThread = new pthread_t;
	rightKeyThread = new pthread_t;
	placeBombThread = new pthread_t;

	pthread_create(upKeyThread, NULL, this->upKey, (void*)this);
	pthread_create(downKeyThread, NULL, this->downKey, (void*)this);
	pthread_create(leftKeyThread, NULL, this->leftKey, (void*)this);
	pthread_create(rightKeyThread, NULL, this->rightKey, (void*)this);
	pthread_create(placeBombThread, NULL, this->place_bomb, (void*)this);

	keyControllerThread = new pthread_t;

	pthread_create(keyControllerThread, NULL, this->keyController, (void*)this);

	cout << "[Player] finished creating"  << endl;
}

Player::~Player(){
	cout << "Trying to destroy player " << this->id+1 << endl;

	this->running = false;

	sem_post(this->keyControllerSemaphore);
	sem_post(this->upKeySemaphore);
	sem_post(this->downKeySemaphore);
	sem_post(this->leftKeySemaphore);
	sem_post(this->rightKeySemaphore);
	sem_post(this->placeBombSemaphore);

	cout << "[Player " << this->id+1 << "] All threads woken" << endl;

	pthread_join(*keyControllerThread, NULL);
	pthread_join(*upKeyThread, NULL);
	pthread_join(*downKeyThread, NULL);
	pthread_join(*leftKeyThread, NULL);
	pthread_join(*rightKeyThread, NULL);
	pthread_join(*placeBombThread, NULL);

	sem_destroy(upKeySemaphore);
	sem_destroy(downKeySemaphore);
	sem_destroy(leftKeySemaphore);
	sem_destroy(rightKeySemaphore);
	sem_destroy(placeBombSemaphore);

	cout << "Player " << this->id+1 << " threads terminated" << endl;

	delete upKeyThread;
	delete downKeyThread;
	delete leftKeyThread;
	delete rightKeyThread;
	delete placeBombThread;
	delete keyControllerThread;

	//SDL_FreeSurface(this->sprite);

	cout << "Player " << this->id + 1 << " destroyed correctly" << endl;
}

void Player::keyPressed(bool* keys){
	this->lastKeyPressed = keys;
	if (!this->isOnline()){
		sem_post(this->keyControllerSemaphore);
	} else{
	//	cout << "[Player " << this->getID() << "] Online player" << endl;
	}
}

int Player::getID() const{
	return this->id;
}

void Player::setCantBombsPlaced(int _cant){
	if (_cant >= 0 && _cant < this->getMaxCantBombs()){
		this->cantBombsPlaced = _cant;
	}
}

int Player::getTicksForBombs() const{
	return this->ticksForBombs;
}

int Player::getPower() const{
	return this->_power;
}

int Player::getMaxCantBombs() const{
	return this->maxCantBombs;
}

void Player::setTicksForBombs(int ticks){
	this->ticksForBombs = ticks;
}

void Player::setPower(int power){
	this->_power = power;
}

void Player::setMaxCantBombs(int cantBombs){
	this->maxCantBombs = cantBombs;
}

void Player::placedOneBomb(){
	this->cantBombsPlaced++;
	//cout << "[Player " << this->getID() << "] [Place bomb] " << "Bombs placed " << this->getCantBomsPlaced() << endl;
}

void Player::oneBombExploded(){
	this->cantBombsPlaced--;
	//cout << "[Player " << this->getID() << "] [explode bomb] " << "Bombs placed " << this->getCantBomsPlaced() << endl;
}

int Player::getCantBombsPlaced() const{
	return this->cantBombsPlaced;
}

void Player::die(){
	if (!this->_dead){
		
		this->_dead = true;
		cout << "Freeing surface" << endl;
		SDL_FreeSurface(this->getSprite());
		cout << "Loading new bmp" << endl;

		this->loadBMP("../Images/Player/player.dead.bmp");
		cout << "Setting color key data" << endl;

	}
}

////////////////////////////////////////////////////////////////////////////////
//////////////////		PRIVATE FUNCTIONS 		////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void* Player::keyController(void* params){
	Player* This = (Player*) params;

	do{
		sem_wait(This->keyControllerSemaphore);
		if (This->running) {

			//cout << "[Player] Pressing \'i\' key: " << (int)This->lastKeyPressed[SDLK_i] << endl;
			if (This->lastKeyPressed[This->UP_KEY] && This->lastKeyPressed[This->LEFT_KEY]){
				This->halfSpeed();
				sem_post(This->upKeySemaphore);
				sem_post(This->leftKeySemaphore);
				This->doubleSpeed();
			}

			else if (This->lastKeyPressed[This->UP_KEY] && This->lastKeyPressed[This->RIGHT_KEY]){
				This->halfSpeed();
				sem_post(This->upKeySemaphore);
				sem_post(This->rightKeySemaphore);
				This->doubleSpeed();
			}

			else if (This->lastKeyPressed[This->DOWN_KEY] && This->lastKeyPressed[This->LEFT_KEY]){
				This->halfSpeed();
				sem_post(This->downKeySemaphore);
				sem_post(This->leftKeySemaphore);
				This->doubleSpeed();
			}

			else if (This->lastKeyPressed[This->DOWN_KEY] && This->lastKeyPressed[This->RIGHT_KEY]){
				This->halfSpeed();
				sem_post(This->downKeySemaphore);
				sem_post(This->rightKeySemaphore);
				This->doubleSpeed();
			}
			else if (This->lastKeyPressed[This->UP_KEY]){
				sem_post(This->upKeySemaphore);
			}
			else if (This->lastKeyPressed[This->DOWN_KEY]){
				sem_post(This->downKeySemaphore);
			}

			else if (This->lastKeyPressed[This->LEFT_KEY]){
				sem_post(This->leftKeySemaphore);
			}
			else if (This->lastKeyPressed[This->RIGHT_KEY]){
				sem_post(This->rightKeySemaphore);
			}

			if (This->lastKeyPressed[This->BOMB_KEY]){
				sem_post(This->placeBombSemaphore);
			}
		} else{
			cout << "[Player "<< This->id+1 << "][KEY_CONTROLLER] no longer running" << endl;
		}
	} while(This->running);

	return NULL;
}

void* Player::upKey(void* params){
	Player* This = (Player*) params;
	double prevY;

	do{
		sem_wait(This->upKeySemaphore);
		//cout << "[PLayer " << This->id+1 <<"][UP] woken up" << endl;
		if (This->running){
			prevY = This->myPosition->getY();
			This->myPosition->setY(prevY - This->getSpeed() * DELTA);

			if (!This->belongsTo->canTheEntityBeThere(This)){
				This->myPosition->setY(prevY);
			} 

			//This->x -= This->speed * DELTA;
			//cout << "Position player N° " << This->id << ": (" << This->x << ", " << This->y << ")" << endl;
		} else{
			cout << "[Player " << This->id+1 << "][UP_KEY] no longer running" << endl;
		}
	} while (This->running);

	return NULL;
}

void* Player::downKey(void* params){
	Player* This = (Player*) params;
	double prevY;

	do{
		sem_wait(This->downKeySemaphore);
		if (This->running){
			prevY = This->myPosition->getY();
			This->myPosition->setY(prevY + This->getSpeed() * DELTA);
			if (!This->belongsTo->canTheEntityBeThere(This)){
				This->myPosition->setY(prevY);
			}
			//cout << "Position player N° " << This->id << ": (" << This->x << ", " << This->y << ")" << endl;
		} else{
			cout << "[Player " << This->id+1 << "][DOWN_KEY] no longer running" << endl;
		}
	} while(This->running);

	return NULL;
}

void* Player::leftKey(void* params){
	Player* This = (Player*) params;
	double prevX;

	do{
		sem_wait(This->leftKeySemaphore);
		if (This->running){
			prevX = This->myPosition->getX();
			This->myPosition->setX(prevX - This->getSpeed() * DELTA);

			if (!This->belongsTo->canTheEntityBeThere(This)){
				This->myPosition->setX(prevX);
			}
			//This->y -= This->speed * DELTA;
		//cout << "Position player N° " << This->id << ": (" << This->x << ", " << This->y << ")" << endl;
		} else{
			cout << "[Player " << This->id+1 << "][LEFT_KEY] no longer running" << endl;
		}
	} while(This->running);

	return NULL;
}

void* Player::rightKey(void* params){
	Player* This = (Player*) params;
	double prevX;

	do{
		sem_wait(This->rightKeySemaphore);
		if (This->running){
			prevX = This->myPosition->getX();
			This->myPosition->setX(prevX + This->getSpeed() * DELTA);
			if (!This->belongsTo->canTheEntityBeThere(This)){
				This->myPosition->setX(prevX);
			}
			//This->y += This->speed * DELTA;
		//cout << "Position player N° " << This->id << ": (" << This->x << ", " << This->y << ")" << endl;
		} else{
			cout << "[Player " << This->id+1 << "][RIGHT_KEY] no longer running" << endl;
		}
	} while(This->running);

	return NULL;
}

void* Player::place_bomb(void* params){
	Player* This = (Player*) params;

	do{
		sem_wait(This->placeBombSemaphore);
		if (This->running && !This->isDead()){
			if (This->getCantBombsPlaced() < This->getMaxCantBombs()){
				bool res;
				res = This->belongsTo->dropBomb(This, This->getX(), This->getY(), This->getTicksForBombs(), This->getPower());

				if (res){
					This->placedOneBomb();
				}
				//cout << "[Player " << This->getID() << "] " << "Bombs placed " << This->getCantBomsPlaced() << endl;
			}
		}
	} while(This->running);

	return NULL;
}

void Player::loadDefaultPosition(){
	this->myPosition->setX( PLAYERS_POSITIONS_X[this->id]);
	this->myPosition->setY( PLAYERS_POSITIONS_Y[this->id]);

	cout << "[Player " << this->id+1 << "] X position: " << this->getX() << endl;
	cout << "[Player " << this->id+1 << "] Y position: " << this->getY() << endl;
	
}

void Player::loadDefaultKeys(){
	this->UP_KEY = UP_KEYS[this->id];
	this->DOWN_KEY = DOWN_KEYS[this->id];
	this->LEFT_KEY = LEFT_KEYS[this->id];
	this->RIGHT_KEY = RIGHT_KEYS[this->id];
	this->BOMB_KEY = BOMB_KEYS[this->id];

	cout << "[Player " << this->id+1 << "] UP: " << this->UP_KEY << endl;
	cout << "[Player " << this->id+1 << "] DOWN: " << this->DOWN_KEY << endl;
	cout << "[Player " << this->id+1 << "] LEFT: " << this->LEFT_KEY << endl;
	cout << "[Player " << this->id+1 << "] RIGHT: " << this->RIGHT_KEY << endl;
	cout << "[Player " << this->id+1 << "] BOMB: " << this->BOMB_KEY << endl;
}

bool Player::isDead() const{
	return this->_dead;
}

int Player::getUpKey() const{
	return this->UP_KEY;
}

int Player::getDownKey() const{
	return this->DOWN_KEY;
}

int Player::getRightKey() const{
	return this->RIGHT_KEY;
}

int Player::getLeftKey() const{
	return this->LEFT_KEY;
}

int Player::getBombKey() const{
	return this->BOMB_KEY;
}

bool Player::isOnline(){
	return this->_online;
}