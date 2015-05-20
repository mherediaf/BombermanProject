#include "Menu.hpp"

#include <iostream>

#include "../GameLoader/GameLoader.hpp"

extern _Window* screen;

using namespace std;

Menu::Menu(_Window* screen){

	this->loadFont = TTF_OpenFont("../Fonts/FreeSans.ttf", 16);
	this->text_color = {0,0,0};

	this->gameStateMutex = new pthread_mutex_t;
	this->eventsBlockMutex = new pthread_mutex_t;

	pthread_mutex_init(this->gameStateMutex, NULL);
	pthread_mutex_init(this->eventsBlockMutex, NULL);

	this->_screen = screen;

	this->continueButton = new SpritedEntity();
	this->loadButton = new SpritedEntity();
	this->PvPButton = new SpritedEntity();
	this->onlineButton = new SpritedEntity();
	this->serverButton = new SpritedEntity();
	this->clientButton = new SpritedEntity();
	this->loadSprite = new SpritedEntity();
	this->IPSprite = new SpritedEntity();

	this->continueButton->loadBMP("../Images/Buttons/continue.bmp");
	this->loadButton->loadBMP("../Images/Buttons/load.bmp");
	this->PvPButton->loadBMP("../Images/Buttons/2players.bmp");
	this->onlineButton->loadBMP("../Images/Buttons/online.bmp");
	this->serverButton->loadBMP("../Images/Buttons/server.bmp");
	this->clientButton->loadBMP("../Images/Buttons/client.bmp");
	this->loadSprite->loadBMP("../Images/saveLayer.bmp");
	this->IPSprite->loadBMP("../Images/IPInput.bmp");

	this->loadBMP("../Images/menubackground.bmp");

	continuePosition.x = 10;
	continuePosition.y = 40;

	loadPosition.x = 160;
	loadPosition.y = 40;

	PVPPosition.x = 10;
	PVPPosition.y = 200;

	onlinePosition.x = 160;
	onlinePosition.y = 200;

	serverPosition.x = 150-65;
	serverPosition.y = 20;

	clientPosition.x = 150-65;
	clientPosition.y = 180;

	this->lastPressedQuit = false;

	this->showMenu();

	this->gameState = MENU_SCREEN;

	this->continueButtonSem = new sem_t;
	this->PVPButtonSem = new sem_t;
	this->onlineButtonSem = new sem_t;
	this->loadButtonSem = new sem_t;
	this->loadSpriteSem = new sem_t;
	this->ipSpriteSem = new sem_t;

	this->serverButtonSem = new sem_t;
	this->clientButtonSem = new sem_t;

	sem_init(this->continueButtonSem, 0, 0);
	sem_init(this->PVPButtonSem, 0, 0);
	sem_init(this->onlineButtonSem, 0, 0);
	sem_init(this->loadButtonSem, 0, 0);
	sem_init(this->loadSpriteSem, 0, 0);
	sem_init(this->ipSpriteSem, 0, 0);

	sem_init(this->serverButtonSem, 0, 0);
	sem_init(this->clientButtonSem, 0, 0);

	this->pressing = false;

	this->continueButtonThread = new pthread_t;
	this->loadButtonThread = new pthread_t;
	this->PvPButtonThread = new pthread_t;
	this->onlineButtonThread = new pthread_t;
	this->serverButtonThread = new pthread_t;
	this->clientButtonThread = new pthread_t;

	this->running = true;

	pthread_create(this->continueButtonThread, NULL, this->continueButtonHandler, this);
	pthread_create(this->loadButtonThread, NULL, this->loadButtonHandler, this);
	pthread_create(this->PvPButtonThread, NULL, this->PvPButtonHandler, this);
	pthread_create(this->onlineButtonThread, NULL, this->onlineButtonHandler, this);
	pthread_create(this->serverButtonThread, NULL, this->serverButtonHandler, this);
	pthread_create(this->clientButtonThread, NULL, this->clientButtonHandler, this);

	this->eventsBlocked = false;

	cout << "Finished menu" << endl;
}

Menu::~Menu(){
	cout << "Destroying menu" << endl << flush;
	this->running = false;
	pthread_mutex_lock(this->gameStateMutex);
	if (this->gameState == PLAYING_GAME){
		pthread_mutex_unlock(this->gameStateMutex);
		delete this->game;
	} else{
		pthread_mutex_unlock(this->gameStateMutex);
	}

	sem_post(this->continueButtonSem);
	sem_post(this->PVPButtonSem);
	sem_post(this->onlineButtonSem);
	sem_post(this->loadButtonSem);
	sem_post(this->serverButtonSem);
	sem_post(this->clientButtonSem);

	pthread_join(*this->continueButtonThread, NULL);
	pthread_join(*this->loadButtonThread, NULL);
	pthread_join(*this->PvPButtonThread, NULL);
	pthread_join(*this->onlineButtonThread, NULL);
	pthread_join(*this->serverButtonThread, NULL);
	pthread_join(*this->clientButtonThread, NULL);

	pthread_mutex_destroy(this->gameStateMutex);
	pthread_mutex_destroy(this->eventsBlockMutex);

	sem_destroy(this->continueButtonSem);
	sem_destroy(this->PVPButtonSem);
	sem_destroy(this->onlineButtonSem);
	sem_destroy(this->loadButtonSem);
	sem_destroy(this->loadSpriteSem);
	sem_destroy(this->ipSpriteSem);
	sem_destroy(this->serverButtonSem);
	sem_destroy(this->clientButtonSem);

	delete this->continueButtonThread;
	delete this->loadButtonThread;
	delete this->PvPButtonThread;
	delete this->onlineButtonThread;
	delete this->serverButtonThread;
	delete this->clientButtonThread;

	delete this->continueButton;
	delete this->loadButton;
	delete this->PvPButton;
	delete this->onlineButton;
	delete this->serverButton;
	delete this->clientButton;
}

void* Menu::continueButtonHandler(void* args){
	
	Menu* This = (Menu*) args;

	while (This->running){
		cout << "[Continue] Awaiting" << endl;
		sem_wait(This->continueButtonSem);

		if (!This->running){
			break;
		} else if (This->mouse_x >= This->continuePosition.x && This->mouse_y >= This->continuePosition.y
				&& This->mouse_x < This->continuePosition.x+130 && This->mouse_y < This->continuePosition.y+80){

			cout << "Pressed (" << This->mouse_x << ", " << This->mouse_y << ")" << endl << flush;
		//	This->gameState = LOADING_GAME;
		//	pthread_mutex_unlock(This->gameStateMutex);

		//	This->game = new Field(2, screen, false);
		//	This->game->setNewGame();
		//	This->game->start();

			GameLoader* loader = new GameLoader();
			string autosaveName;
			autosaveName = "autosave";

			pthread_mutex_lock(This->gameStateMutex);
			This->game = loader->load(autosaveName, This);
			if (This->gameState == MENU_SCREEN){
				This->game->start();
		//		pthread_mutex_lock(This->gameStateMutex);
				This->gameState = PLAYING_GAME;
			} else{
				cout << "Error loading autosave file" << endl;
			//	pthread_mutex_lock(This->gameStateMutex);
			//	This->gameState = PLAYING_GAME;
			//	pthread_mutex_unlock(This->gameStateMutex);
			}
			pthread_mutex_unlock(This->gameStateMutex);

			
		}

	}
	return NULL;
}

void* Menu::loadButtonHandler(void* args){
	Menu* This = (Menu*) args;

	while (This->running){
		cout << "[Load] Awaiting" << endl;
		sem_wait(This->loadButtonSem);

		if (!This->running){
			break;
		} else if (This->mouse_x >= This->loadPosition.x && This->mouse_y >= This->loadPosition.y
				&& This->mouse_x < This->loadPosition.x+130 && This->mouse_y < This->loadPosition.y+80){

			cout << "Pressed (" << This->mouse_x << ", " << This->mouse_y << ")" << endl << flush;

			pthread_mutex_lock(This->gameStateMutex);
			This->gameState = LOADING_GAME;
			This->showLoadingScreen();
			pthread_mutex_unlock(This->gameStateMutex);

			This->deploySaveScreen();

			
		}

	}
	return NULL;
}

void* Menu::PvPButtonHandler(void* args){
	Menu* This = (Menu*) args;

	while (This->running){
		cout << "[PVP] Awaiting" << endl;
		sem_wait(This->PVPButtonSem);

		if (!This->running){
			break;
		} else if (This->mouse_x >= This->PVPPosition.x && This->mouse_y >= This->PVPPosition.y
				&& This->mouse_x < This->PVPPosition.x+130 && This->mouse_y < This->PVPPosition.y+80){

			cout << "Pressed (" << This->mouse_x << ", " << This->mouse_y << ")" << endl << flush;
		//	pthread_mutex_lock(This->gameStateMutex);
		//	This->gameState = LOADING_GAME;
		//	pthread_mutex_unlock(This->gameStateMutex);

			This->game = new Field(2, screen, false, 0, This, NULL);
			This->game->setNewGame();
			This->game->start();

			pthread_mutex_lock(This->gameStateMutex);
			This->gameState = PLAYING_GAME;

	//		This->blockEvents();

			pthread_mutex_unlock(This->gameStateMutex);
			
		}

	}

	return NULL;
}

void* Menu::onlineButtonHandler(void* args){
	Menu* This = (Menu*) args;

	while (This->running){
		cout << "[PVP] Awaiting" << endl;
		sem_wait(This->onlineButtonSem);

		if (!This->running){
			break;
		} else if (This->mouse_x >= This->onlinePosition.x && This->mouse_y >= This->onlinePosition.y
				&& This->mouse_x < This->onlinePosition.x+130 && This->mouse_y < This->onlinePosition.y+80){

			cout << "Pressed (" << This->mouse_x << ", " << This->mouse_y << ")" << endl << flush;
			pthread_mutex_lock(This->gameStateMutex);
			This->showServerOrClient();
			This->gameState = CHOOSING_SERVER_OR_CLIENT;
			pthread_mutex_unlock(This->gameStateMutex);

		}

	}

	return NULL;
}

void* Menu::serverButtonHandler(void* args){
	Menu* This = (Menu*) args;

	while (This->running){
		cout << "[Server] Awaiting" << endl;
		sem_wait(This->serverButtonSem);

		if (!This->running){
			break;
		} else if (This->mouse_x >= This->serverPosition.x && This->mouse_y >= This->serverPosition.y
				&& This->mouse_x < This->serverPosition.x+130 && This->mouse_y < This->serverPosition.y+80){

			cout << "Pressed (" << This->mouse_x << ", " << This->mouse_y << ")" << endl << flush;
			pthread_mutex_lock(This->gameStateMutex);
			This->showAwaitingConnection();
			This->gameState = AWAITING_CLIENT_CONNECTION;
			pthread_mutex_unlock(This->gameStateMutex);

			TcpServer* server = new TcpServer();
			Connection* conn = server->Accept();

			if (conn != NULL){
				cout << "[Server] Connection stablished" << endl;
				This->game = new Field(2, This->getScreen(), true, 0, This, conn);
				This->game->setNewGame();
				This->game->start();
			}

		}

	}

	return NULL;
}

void* Menu::clientButtonHandler(void* args){
	Menu* This = (Menu*) args;

	while (This->running){
		cout << "[Server] Awaiting" << endl;
		sem_wait(This->clientButtonSem);

		if (!This->running){
			break;
		} else if (This->mouse_x >= This->clientPosition.x && This->mouse_y >= This->clientPosition.y
				&& This->mouse_x < This->clientPosition.x+130 && This->mouse_y < This->clientPosition.y+80){

			cout << "Pressed (" << This->mouse_x << ", " << This->mouse_y << ")" << endl << flush;
			pthread_mutex_lock(This->gameStateMutex);
			This->gameState = CHOOSING_SERVER_IP;
			pthread_mutex_unlock(This->gameStateMutex);
			This->showIPConnection();

		}

	}

	return NULL;
}


_Window* Menu::getScreen(){
	return this->_screen;
}

void Menu::keyPressed(bool* keys, SDL_Event* event){
	bool nowPressing = (event->type == SDL_MOUSEBUTTONDOWN);

	pthread_mutex_lock(this->gameStateMutex);
	int state = this->gameState;
	pthread_mutex_unlock(this->gameStateMutex);

	this-> mouse_x = event->button.x;
	this-> mouse_y = event->button.y;

	bool quit = (keys[SDLK_ESCAPE] || event->type == SDL_QUIT);

	if (state == LOADING_GAME){
	//	cout << "Detected loading" << endl;
		this->_event = event;
		this->_keys = keys;
		sem_post(this->loadSpriteSem);
	} else if (state == CHOOSING_SERVER_IP){
	//	cout << "<<<<" << endl;
		this->_event = event;
		this->_keys = keys;
		sem_post(this->ipSpriteSem);

		/*
		if (quit){
			pthread_mutex_lock(this->gameStateMutex);
			this->gameState = CHOOSING_SERVER_OR_CLIENT;
			this->showServerOrClient();
			pthread_mutex_unlock(this->gameStateMutex);
		}
		*/

	} else if (state == PLAYING_GAME){
	//	cout << "Detected playing" << endl;
		this->game->keyPressed(keys, event);
	} else{

		// if (!this->pressing && nowPressing)
		switch(state){
			case MENU_SCREEN:{
				if (!this->pressing && nowPressing){
		//			cout << "----" << endl;
					sem_post(this->continueButtonSem);
					sem_post(this->PVPButtonSem);
					sem_post(this->onlineButtonSem);
					sem_post(this->loadButtonSem);
				}
				break;
			}

		//	case PLAYING_GAME:{
		//		break;
		//	}

			case LOADING_GAME:{
				break;
			}

			case CHOOSING_SERVER_OR_CLIENT:{
				if (quit && !this->lastPressedQuit){
					pthread_mutex_lock(this->gameStateMutex);
					this->gameState = MENU_SCREEN;
					this->showMenu();
					pthread_mutex_unlock(this->gameStateMutex);
				} else if (!this->pressing && nowPressing){
					sem_post(this->serverButtonSem);
					sem_post(this->clientButtonSem);
				}
				break;
			}

			case AWAITING_CLIENT_CONNECTION:{
				if (quit){
					pthread_mutex_lock(this->gameStateMutex);
					this->gameState = CHOOSING_SERVER_OR_CLIENT;
					this->showServerOrClient();
					pthread_mutex_unlock(this->gameStateMutex);
				}
				break;
			}

			case CHOOSING_SERVER_IP:{
				if (quit){
				
				}
				break;
			}

			default:{
				cout << "Menu state unknown" << endl;
			}
		}

	}

	this->pressing = nowPressing;
	this->lastPressedQuit = quit;

}

void Menu::blockEvents(){
	pthread_mutex_lock(this->eventsBlockMutex);
	this->eventsBlocked = true;
	pthread_mutex_unlock(this->eventsBlockMutex);
}

void Menu::unblockEvents(){
	pthread_mutex_lock(this->eventsBlockMutex);
	this->eventsBlocked = false;
	pthread_mutex_unlock(this->eventsBlockMutex);
}

bool Menu::eventsAreBlocked(){
	return this->eventsBlocked;
}

bool Menu::finishWithEscape(){
	return this->gameState == MENU_SCREEN;
}

void Menu::showMenu(){
	this->getScreen()->addLayer(this->getSprite(), NULL, NULL);
	this->getScreen()->addLayer(this->continueButton->getSprite(), NULL, &continuePosition);
	this->getScreen()->addLayer(this->loadButton->getSprite(), NULL, &loadPosition);
	this->getScreen()->addLayer(this->PvPButton->getSprite(), NULL, &PVPPosition);
	this->getScreen()->addLayer(this->onlineButton->getSprite(), NULL, &onlinePosition);

	this->getScreen()->show();
	cout << "Showed menu" << endl;
}

void Menu::showServerOrClient(){
	this->getScreen()->addLayer(this->getSprite(), NULL, NULL);
	this->getScreen()->addLayer(this->serverButton->getSprite(), NULL, &this->serverPosition);
	this->getScreen()->addLayer(this->clientButton->getSprite(), NULL, &this->clientPosition);

	this->getScreen()->show();
	cout << "Showing server cliente buttons" << endl;
}

void Menu::showAwaitingConnection(){
	this->getScreen()->addLayer(this->getSprite(), NULL, NULL);
	
	this->getScreen()->show();
}

void Menu::destroyField(){
	pthread_mutex_lock(this->gameStateMutex);
	if (this->game != NULL){
		delete this->game;
		this->game = NULL;
		this->gameState = MENU_SCREEN;
		this->showMenu();
	}
	pthread_mutex_unlock(this->gameStateMutex);	
}

void Menu::showLoadingScreen(){
	SDL_Rect backgroundPosition;
	backgroundPosition.x = 0;
	backgroundPosition.y = 20;

//	SDL_Rect fontPosition;
//	fontPosition.x = 100;
//	fontPosition.y = 100+20;

	this->getScreen()->addLayer(this->loadSprite->getSprite(), NULL, &backgroundPosition);
	this->getScreen()->show();

	cout << "Showing loading screen" << endl;
}



void Menu::deploySaveScreen(){

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
		this->getScreen()->addLayer(this->loadSprite->getSprite(), NULL, &backgroundPosition);
		text = TTF_RenderText_Solid(font, &name[0], text_color);
		if (text == NULL)
		{
			cerr << "TTF_RenderText_Solid() Failed: " << TTF_GetError() << endl;
			TTF_Quit();
			return;
		}
		this->getScreen()->addLayer(text, NULL, &fontPosition);

	//	if (SDL_BlitSurface(this->getScreen()->getSprite(), NULL, text, &fontPosition) != 0){
	//		cout << "Error displaying text" << endl;
	//		cout << TTF_GetError() << endl;
	//	}
		this->getScreen()->show();

	//	cout << "Save detected key" << endl;
	//	cout << counter << endl;
	//	counter++;
		bool found;
		sem_wait(this->loadSpriteSem);



		if (this->_event->type == SDL_KEYDOWN){

	//		if (!pressing){

				if (this->_keys[SDLK_ESCAPE] || this->_event->type == SDL_QUIT){
					this->gameState = MENU_SCREEN;
					this->showMenu();
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
					//	this->saver->save(saveName, this->_amountOfPlayers, this->thePlayers, this->theUnbreakableWalls,
					//	this->theBreakableWalls, this->theBombs, this->theFlames);

						GameLoader* loader = new GameLoader();
						this->game = loader->load(saveName, this);
						if (this->game != NULL){
							this->game->start();
							this->gameState = PLAYING_GAME;
						} else{
							cout << "Loading failed" << endl;
							this->gameState = MENU_SCREEN;
							this->showMenu();
						}

						break;
					}
				}

				if (!pressedAny || !this->_keys[lastPressed]){

					if (this->_keys[SDLK_BACKSPACE]){
						if (pointerPosition > 1){
							pointerPosition--;

							name[pointerPosition] = '\0';

						//	cout << "Backspace" << endl;

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



void Menu::showIPConnection(){

//	if (TTF_Init() != 0)
//	{
//		cerr << "TTF_Init() Failed: " << TTF_GetError() << endl;
//		return;
//	}

	char name[17];
	for (int i = 1; i < 17; ++i)
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
		this->getScreen()->addLayer(this->IPSprite->getSprite(), NULL, &backgroundPosition);
		text = TTF_RenderText_Solid(font, &name[0], text_color);
		if (text == NULL)
		{
			cerr << "TTF_RenderText_Solid() Failed: " << TTF_GetError() << endl;
			TTF_Quit();
			return;
		}
		this->getScreen()->addLayer(text, NULL, &fontPosition);

	//	if (SDL_BlitSurface(this->getScreen()->getSprite(), NULL, text, &fontPosition) != 0){
	//		cout << "Error displaying text" << endl;
	//		cout << TTF_GetError() << endl;
	//	}
		this->getScreen()->show();

	//	cout << "Save detected key" << endl;
	//	cout << counter << endl;
	//	counter++;
		bool found;
		sem_wait(this->ipSpriteSem);



		if (this->_event->type == SDL_KEYDOWN){

	//		if (!pressing){

				if (this->_keys[SDLK_ESCAPE] || this->_event->type == SDL_QUIT){
					this->gameState = MENU_SCREEN;
					this->showMenu();
					break;
				}

				if (this->_keys[SDLK_RETURN]){
					if (pointerPosition > 1){

						char aux[16];
						for (int i = 0; i < 16; ++i)
						{
							aux[i] = name[i+1];
						}

						TcpClient* client = new TcpClient();

						Connection* conn = client->Connect(aux, PORT);

						if (conn != NULL){
							this->game = new Field(2, this->getScreen(), true, 1, this, conn);
							this->game->setNewGame();
							this->game->start();

							pthread_mutex_lock(this->gameStateMutex);
							this->gameState = PLAYING_GAME;
							pthread_mutex_unlock(this->gameStateMutex);
						} else{

							pthread_mutex_lock(this->gameStateMutex);
							this->gameState = MENU_SCREEN;
							this->showMenu();
							pthread_mutex_unlock(this->gameStateMutex);
						}

						break;
					}
				}

				if (!pressedAny || !this->_keys[lastPressed]){

					if (this->_keys[SDLK_BACKSPACE]){
						if (pointerPosition > 1){
							pointerPosition--;

							name[pointerPosition] = '\0';

						//	cout << "Backspace" << endl;

							pressedAny = false;
						//	lastPressed = SDLK_BACKSPACE;
						}
					}

					found = false;

					for (int i = SDLK_0; !found && i <= SDLK_9; ++i)
					{
						if (this->_keys[i]){
							if (pointerPosition < 16){
								name[pointerPosition] = '0' - SDLK_0 + i;
								pointerPosition++;
							//	cout << "0" << endl;
								pressedAny = true;
								lastPressed = i;

								found = true;
							}
						}
					}

					if (this->_keys[SDLK_PERIOD]){
						if (pointerPosition < 16){
							name[pointerPosition] = '.';
							pointerPosition++;
							pressedAny = true;
							lastPressed = SDLK_PERIOD;

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