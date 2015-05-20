#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "../ConnectionIncludes/connectionIncludes.hpp"
#include "../GameEntity/GameEntity.hpp"

#include <string>

class Connection: public GameEntity
{

	private:

		fd_t fd_other_end;

		pthread_mutex_t* open_lock;
		bool open;
		bool is_open() const;

	//	phtread_t* recieve_handler_thread;
	//	void* recieve_handler(void* args);
	//	void recieve();

		//std::string recieve_buffer;
		char recieve_buffer[BUFFER_SIZE+1];
	//	std::string message_recieved;

	public:
		Connection(fd_t file_descriptor);
		~Connection();

		std::string* Recieve();
		void Send(std::string* message);

		void Close();
	
};

#endif