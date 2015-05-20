#ifndef CONSOLE_COMMAND_HPP
#define CONSOLE_COMMAND_HPP value

#include "../GameEntity/GameEntity.hpp"

#include <string>

class ConsoleCommand: public GameEntity
{
	private:

		bool _commandSet;
		bool commandIsSet();

		bool _nameSet;
		bool nameIsSet();

		bool _executed;
		bool executed();

		std::string _logNameFile;
		std::string _command;

	public:
		ConsoleCommand();
		~ConsoleCommand();

		void setLogNameFile(std::string& namefile);
		void setCommand(std::string& comm);
		std::string getLogNameFile();
		std::string getCommand();

		bool execute();
		std::string getResult();


	
};


#endif