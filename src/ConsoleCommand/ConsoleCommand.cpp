#include "ConsoleCommand.hpp"

#include <iostream>
#include <stdlib.h>
#include <fstream>

using namespace std;

ConsoleCommand::ConsoleCommand(){
	this->_nameSet = false;
	this->_commandSet = false;
	this->_executed = false;
}

ConsoleCommand::~ConsoleCommand(){

}


void ConsoleCommand::setLogNameFile(string& namefile){
	this->_logNameFile = namefile;
	this->_nameSet = true;
}

void ConsoleCommand::setCommand(string&	 comm){
	this->_command = comm;
	this->_commandSet = true;
}

string ConsoleCommand::getLogNameFile(){
	return this->_logNameFile;
}

string ConsoleCommand::getCommand(){
	return this->_command;
}

bool ConsoleCommand::execute(){
	if (!this->executed() && this->nameIsSet() && this->commandIsSet()){
		int commResult;

		string comm;

		comm = this->getCommand() + " >> " + this->getLogNameFile() + "\n";

	//	cout << "Command: " << comm << endl;

		commResult = system(comm.c_str());

		this->_executed = true;

		return commResult!=-1;
	} else{
		return false;
	}
}

string ConsoleCommand::getResult(){
	string result;

	result = "";

	if (this->executed()){


		ifstream logFile;

		logFile.open(this->getLogNameFile().c_str(), ios_base::in);

		if (logFile.good()){
			int MAX_LINE_SIZE = 1024;

			char buffer[1025];
			buffer[1024] = '\0';

			while( !logFile.eof() ){
				logFile.getline(buffer, MAX_LINE_SIZE);

				result+=buffer;
				result+="\n";

			}


			logFile.close();

			string destroy;
			destroy = "rm -f " + this->getLogNameFile();

			cout << "Destroy comm: " << destroy << endl;

			int outDelete;
			outDelete = system(destroy.c_str());
			if (outDelete == -1){
				cout << "[Command] Error destroying log" << endl;
			}
		}
	}

	return result;
}



bool ConsoleCommand::nameIsSet(){
	return this->_nameSet;
}

bool ConsoleCommand::commandIsSet(){
	return this->_commandSet;
}

bool ConsoleCommand::executed(){
	return this->_executed;
}