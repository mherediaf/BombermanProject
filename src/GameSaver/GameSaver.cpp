#include "GameSaver.hpp"

#include <fstream>
#include <string>
#include <map>

using namespace std;

GameSaver::GameSaver(): GameEntity(){
	this->console = new ConsoleCommand();
}

GameSaver::~GameSaver(){
	delete this->console;
}


void GameSaver::save(string& nameFile, int numPlayers, Player** thePlayers, list< UnbreakableWall* >* theUnbreakableWalls,
			list< BreakableWall* >* theBreakableWalls, list< Bomb* >* theBombs, list< Flame* >* theFlames){

	string outputName;
	string outputZip;

	string rootPath;

	rootPath = "../saves/";

	outputName = rootPath + nameFile+".txt";
	outputZip = rootPath + nameFile+".zip";

	ofstream outputFile;

	outputFile.open(outputName.c_str(), ios_base::out);

	if (outputFile.is_open()){
		//the saving
		
		// Amount of players
		outputFile << numPlayers << endl;

		map< Player*, int> playerToId;
		map< Bomb*, int> bombToId;

		for (int i = 0; i < numPlayers; ++i)
		{
			playerToId[ thePlayers[i] ] = i;
		}

		list< Bomb* >::iterator itBmb = theBombs->begin();
		for (int i = 0; i < (int)theBombs->size(); ++i)
		{
			bombToId[ *itBmb ] = i;
			itBmb++;
		}

		int x, y;
		for (int i = 0; i < numPlayers; ++i)
		{
			Player* thisPlayer = thePlayers[i];

			x = thisPlayer->getX();
			y = thisPlayer->getY();

			// player position
			outputFile << x << " " << y << endl;

			// Print the keys
		//	outputFile << thisPlayer->getUpKey() << endl;
		//	outputFile << thisPlayer->getDownKey() << endl;
		//	outputFile << thisPlayer->getRightKey() << endl;
		//	outputFile << thisPlayer->getLeftKey() << endl;
		//	outputFile << thisPlayer->getBombKey() << endl;

			outputFile << thisPlayer->getTicksForBombs() << endl;
			outputFile << thisPlayer->getPower() << endl;
			outputFile << thisPlayer->getMaxCantBombs() << endl;

			outputFile << thisPlayer->getCantBombsPlaced() << endl;

			if (thisPlayer->isDead()){
				outputFile << "1" << endl;
			} else{
				outputFile << "0" << endl;
			}

			outputFile << *(thisPlayer->getSpritePath()) << endl;

		}

		// Write the Unbreakable Walls
		outputFile << theUnbreakableWalls->size() << endl;

		for( list< UnbreakableWall*>::iterator itUW = theUnbreakableWalls->begin();
				itUW != theUnbreakableWalls->end(); itUW++){


			x = (*itUW)->getX();
			y = (*itUW)->getY();

			outputFile << x << " " << y << endl;
			outputFile << *((*itUW)->getSpritePath()) << endl;
		}

// The brekable walls
		outputFile << theBreakableWalls->size() << endl;

		for( list< BreakableWall*>::iterator itBW = theBreakableWalls->begin();
				itBW != theBreakableWalls->end(); itBW++){


			x = (*itBW)->getX();
			y = (*itBW)->getY();

			outputFile << x << " " << y << endl;
			outputFile << *((*itBW)->getSpritePath()) << endl;
		}

// The bombs
		outputFile << theBombs->size() << endl;

		for( list< Bomb*>::iterator itBomb = theBombs->begin();
				itBomb != theBombs->end(); itBomb++){


			x = (*itBomb)->getX();
			y = (*itBomb)->getY();

			outputFile << x << " " << y << endl;

			int power;
			power = (*itBomb)->getPower();
			outputFile << power << endl;
			
			outputFile << *((*itBomb)->getSpritePath()) << endl;

			outputFile << (*itBomb)->amountOfTicksLeft() << endl;

			bool playerLeft = (*itBomb)->HasThePlayerLeftTheArea();
			if (playerLeft){
				outputFile << "1" << endl;
			} else{
				outputFile << "0" << endl;
			}

			Player* from = (*itBomb)->playerThatGeneratedTheBomb();

			bool activated = (*itBomb)->isActivated();
			if (activated){
				outputFile << "1" << endl;
			} else{
				outputFile << "0" << endl;
			}

			outputFile << playerToId[from] << endl;
		}

	// The flames

		outputFile << theFlames->size() << endl;

		for( list< Flame*>::iterator itFlame = theFlames->begin();
				itFlame != theFlames->end(); itFlame++){


			x = (*itFlame)->getX();
			y = (*itFlame)->getY();

			outputFile << x << " " << y << endl;
			outputFile << *((*itFlame)->getSpritePath()) << endl;

			Bomb* from = (*itFlame)->getBombFrom();

			outputFile << bombToId[ from ] << endl;

			outputFile << (*itFlame)->amountOfTicksLeft() << endl;
		}

		// Finished writing


		outputFile.close();

		string commandZip;
		commandZip = "zip " + outputZip + " " + outputName;
		string outputResult;
		outputResult = "../Log/savelog.txt";

		this->console->setCommand(commandZip);
		this->console->setLogNameFile(outputResult);

		if (this->console->execute()){

			delete this->console;
			this->console = new ConsoleCommand();


			string commandDelete;
			commandDelete = "rm -f " + outputName;

			this->console->setCommand(commandDelete);
			this->console->setLogNameFile(outputResult);

			if (this->console->execute()){

			}

		}

		delete this->console;
		this->console = new ConsoleCommand();

	//	system(commandZip.c_str());


	//	system(commandDelete.c_str());
	}



}