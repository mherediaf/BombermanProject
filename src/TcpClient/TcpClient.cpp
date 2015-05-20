#include "TcpClient.hpp"

#include <iostream>

using namespace std;

TcpClient::TcpClient(): BaseSocket(AF_INET, SOCK_STREAM, 0){
}

TcpClient::~TcpClient(){

}

Connection* TcpClient::Connect(char* ip, int port){
	if (this->socketIsOpened()){
		this->remote.sin_family = AF_INET;
		this->remote.sin_port = htons(port);

		if (inet_aton(ip, &(remote.sin_addr) ) != 0){
			int connecting;
			connecting = connect(this->sock, (struct sockaddr*)&(this->remote), sizeof(this->remote));
			if (connecting == 0){
				Connection* res = new Connection(this->sock);
				return res;
			} else{
				cout << "Error connecting" << endl;
				return NULL;
			}
		} else{
			cout << "Error wrong IP";
			return NULL;
		}
	} else{
		return NULL;
	}
}
