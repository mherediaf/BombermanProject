#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include "../BaseSocket/BaseSocket.hpp"
#include "../Connection/Connection.hpp"

#include <list>
#include <string>
#include <pthread.h>
#include <semaphore.h>

class TcpServer: public BaseSocket
{
	private:
	//	int sock;
		struct sockaddr_in name;
		struct sockaddr_in ctl_addr;

	//	pthread_mutex_t availablePositionMutex;
	//	int getFirstAvailablePosition();


		void start();

		bool started_correctly;
	//	bool socket_open;
		bool startedCorrectly() const;
	//	bool socketIsOpended() const;

	public:
		TcpServer();
		~TcpServer();

		Connection* Accept();

		//std::string recieve();
};

#endif
