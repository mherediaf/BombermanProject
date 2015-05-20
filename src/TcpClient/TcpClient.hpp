#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include "../BaseSocket/BaseSocket.hpp"
#include "../Connection/Connection.hpp"

class TcpClient: public BaseSocket
{
	private:
	//	int sock;
	//	bool socket_correct;
		struct sockaddr_in remote;

	//	bool socketIsCorrect() const;

	public:
		TcpClient();
		~TcpClient();

		Connection* Connect(char* ip, int port);
	
};

#endif