#ifndef BASE_SOCKET_HPP
#define BASE_SOCKET_HPP

#include "../ConnectionIncludes/connectionIncludes.hpp"
#include "../GameEntity/GameEntity.hpp"

class BaseSocket: public GameEntity
{
	protected:
		int sock;

		bool socket_open;
		bool socketIsOpened() const;

	public:
		BaseSocket(int domain, int type, int protocol);
		virtual ~BaseSocket();
	
};

#endif