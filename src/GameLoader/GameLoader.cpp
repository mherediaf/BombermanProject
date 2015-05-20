#include "GameLoader.hpp"

#include <fstream>
#include <list>
#include <map>
#include <iostream>
#include <unistd.h>

#include "../ConsoleCommand/ConsoleCommand.hpp"

using namespace std;

extern _Window* screen;

GameLoader::GameLoader(): GameEntity(){

}

GameLoader::~GameLoader(){

}


Field* GameLoader::load(string& nameFile, Menu* parent){
	Field* newGame;
	newGame = NULL;

	string rootPath;
	rootPath = "../saves/";

	ifstream inFile;

	string zipFile = rootPath + nameFile + ".zip";

	string unzipCommand;
	unzipCommand = "unzip " + zipFile;

	string outputCommand;
	outputCommand = "/dev/null";

	ConsoleCommand* commander = new ConsoleCommand();
	commander->setCommand(unzipCommand);
	commander->setLogNameFile(outputCommand);

	cout << unzipCommand << endl;

	if (commander->execute()){
		cout << "Unzipped correctly" << endl;

//		sleep(6);
	

//	system(unzipCommand.c_str());

		string txtFile;
		txtFile = "saves/" + nameFile + ".txt";

		cout << "Opening " << txtFile << endl;


		inFile.open(txtFile.c_str(), ios_base::in);

		if (inFile.is_open()){
			// Start reading and creating the necessary objects


			// Read players data
			int amount_of_players;

			inFile >> amount_of_players;
			newGame = new Field(amount_of_players, screen, false, 0, parent, NULL);

			Player** players = new Player*[amount_of_players];
			int x, y;
			string spritePath;

			map< int, Bomb* > idToBomb;

			for (int i = 0; i < amount_of_players; ++i)
			{
				players[i] = new Player(i, false, newGame);

				inFile >> x >> y;

				players[i]->setX(x);
				players[i]->setY(y);

				int ticksForBombs;
				inFile >> ticksForBombs;
				players[i]->setTicksForBombs(ticksForBombs);

				int power;
				inFile >> power;
				players[i]->setPower(power);

				int maxCantBombs;
				inFile >> maxCantBombs;
				players[i]->setMaxCantBombs(maxCantBombs);

				int cantBombsPlaced;
				inFile >> cantBombsPlaced;
				players[i]->setCantBombsPlaced(cantBombsPlaced);

				int dead;
				inFile >> dead;
				if (dead == 1){ //Player is dead
					players[i]->die();
				}

				inFile >> spritePath;
				players[i]->loadBMP(spritePath);

			}


			// Read UnbreakableWalls data
			int amountUnbreakableWalls;
			inFile >> amountUnbreakableWalls;

			list< UnbreakableWall* >* theUnbreakableWalls = new list< UnbreakableWall* >;

			for (int i = 0; i < amountUnbreakableWalls; ++i)
			{
				inFile >> x >> y;
				UnbreakableWall* ubWall = new UnbreakableWall();

				ubWall->setX(x);
				ubWall->setY(y);

				inFile >> spritePath;
				ubWall->loadBMP(spritePath);

				theUnbreakableWalls->push_back(ubWall);

			}


			// Read BreakableWalls data
			int amountBreakableWalls;
			inFile >> amountBreakableWalls;

			list< BreakableWall* >* theBreakableWalls = new list< BreakableWall* >;

			for (int i = 0; i < amountBreakableWalls; ++i)
			{
				inFile >> x >> y;
				BreakableWall* bWall = new BreakableWall();

				bWall->setX(x);
				bWall->setY(y);

				inFile >> spritePath;
				bWall->loadBMP(spritePath);

				theBreakableWalls->push_back(bWall);

			}


			// Read Bombs data
			int amountBombs;
			inFile >> amountBombs;

			list< Bomb* >* theBombs = new list< Bomb* >;

			for (int i = 0; i < amountBombs; ++i)
			{
				inFile >> x >> y;

				int power;
				inFile >> power;

				inFile >> spritePath;

				int ticksLeft;
				inFile >> ticksLeft;

				int playerLeftArea;
				inFile >> playerLeftArea;

				int idFrom;
				inFile >> idFrom;
				Player* playerFrom = players[idFrom];

				int activated;
				inFile >> activated;


				Bomb* bomb = new Bomb(ticksLeft, x, y, power, playerFrom, screen);
				bomb->loadBMP(spritePath);
			//	bomb->setX(x);
			//	bomb->setY(y);
			//	bomb->setTicks(ticksLeft);
				if (playerLeftArea == 1){
					bomb->playerLeftArea();
				}
				if (activated){
					bomb->activate();
				}

				theBombs->push_back(bomb);

				idToBomb[ i ] = bomb;

			}



			// Read Flames data
			int amountFlames;
			inFile >> amountFlames;

			list< Flame* >* theFlames = new list< Flame* >;

			for (int i = 0; i < amountFlames; ++i)
			{
				inFile >> x >> y;

			//	flame->setX(x);
			//	flame->setY(y);

				inFile >> spritePath;

				int idBombFrom;
				inFile >> idBombFrom;
				Bomb* from = idToBomb[idBombFrom];

				int ticksLeft;
				inFile >> ticksLeft;

				Flame* flame = new Flame(x, y, screen, from);
				flame->loadBMP(spritePath);
				flame->setTicks(ticksLeft);

				theFlames->push_back(flame);

			}
			

			// Finish reading

			inFile.close();

			newGame->setPlayers(players, amount_of_players);
			newGame->setUnbreakableWalls(theUnbreakableWalls);
			newGame->setBreakableWalls(theBreakableWalls);
			newGame->setBombs(theBombs);
			newGame->setFlames(theFlames);

			delete commander;
			commander = new ConsoleCommand();

			string deleteCommand;
			deleteCommand = "rm -rf saves/";

			commander->setCommand(deleteCommand);
			commander->setLogNameFile(outputCommand);

			if (commander->execute()){
				cout << "File deleted successfully" << endl;
			} else{
				cout << "Error: Deleting file" << endl;
			}

		} else{
			cout << "Error: Loading text file" << endl;;
		}

	} else{
		cout << "Error: Unziping" << endl;
	}

	delete commander;

	return newGame;
}