#include "TcpServer.hpp"

#include <iostream>

using namespace std;

TcpServer::TcpServer(): BaseSocket(AF_INET, SOCK_STREAM, 0){
	this->start();
}

TcpServer::~TcpServer(){

}

Connection* TcpServer::Accept(){
	int sock_ctrl;

	int t = sizeof(this->ctl_addr);
	cout << "accepting" << endl;
	sock_ctrl = accept(this->sock, (struct sockaddr*) &(this->ctl_addr), (socklen_t*)&t);
	if (sock_ctrl == -1){
		cout << "Error accepting connection" << endl;
		return NULL;
	} else{
		cout << "Accepted correctly" << endl;
		Connection* res = new Connection(sock_ctrl);
		return res;
	}
}

void TcpServer::start(){
	this->started_correctly = this->socketIsOpened();

//	this->sock=socket(AF_INET, SOCK_STREAM, 0);
//	this->started_correctly = this->started_correctly && ( this->sock >= 0 );

	if (this->started_correctly){
//		this->socket_open=true;

		this->name.sin_family = AF_INET;
		this->name.sin_addr.s_addr = INADDR_ANY;
		this->name.sin_port = htons(PORT);
		cout << "[Server] Binding to port: " << PORT << endl;

		int binding = bind(this->sock, (struct sockaddr*) &(this->name), sizeof(this->name));

		this->started_correctly = this->started_correctly && ( binding == 0);

		if (this->started_correctly){
			int listening = listen(this->sock, 1);
			this->started_correctly = this->started_correctly && ( listening == 0 );
			if (!this->started_correctly){
				cout << "Error listening" << endl;
			}
		} else{
			cout << "Error binding" << endl;
		}
	} else{
		cout << "Error creating socket" << endl;
	}
}

bool TcpServer::startedCorrectly() const{
	return this->started_correctly;
}

/*
bool TcpServer::socketIsOpened() const{
	return this->socket_open;
}
*/