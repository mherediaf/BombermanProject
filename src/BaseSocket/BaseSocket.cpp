#include "BaseSocket.hpp"
#include <iostream>

using namespace std;

BaseSocket::BaseSocket(int domain, int type, int protocol): GameEntity(){
	this->sock = socket(domain, type, protocol);
	this->socket_open = this->sock >= 0;

	if (this->socketIsOpened()){
		cout << "Created socket: " << this->sock << endl;
	}
}

BaseSocket::~BaseSocket(){
	if (this->socketIsOpened()){
		cout << "Closing socket: " << this->sock << endl;
		close(this->sock);
	}
}


bool BaseSocket::socketIsOpened() const{
	return this->socket_open;
}