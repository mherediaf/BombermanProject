#include "Connection.hpp"

#include <iostream>
#include <string.h>

using namespace std;


Connection::Connection(fd_t file_descriptor): GameEntity(){
	this->fd_other_end = file_descriptor;
	this->open = true;

	this->recieve_buffer[BUFFER_SIZE] = '\0';
	this->open_lock = new pthread_mutex_t;
	pthread_mutex_init(this->open_lock, NULL);

	//this->recieve_handler_thread = new pthread_t;
	//pthread_create( this->recieve_handler_thread, NULL, this->recieve_handler, NULL );
}

Connection::~Connection(){
	//this->open_lock->lock();
	pthread_mutex_lock( this->open_lock );
	if (this->is_open()){
		//this->open_lock->unlock();
		pthread_mutex_unlock( this->open_lock );
		this->Close();
	} else{
		//this->open_lock->unlock();
		pthread_mutex_unlock( this->open_lock );
	}
}


void Connection::Send(string* message){
	cout << "Sending text: " << *message << " to fd: " << this->fd_other_end << endl;

	int size_sent;

	size_sent = send(this->fd_other_end, message->c_str(), (size_t)(message->size()), 0);
}

//void Connection::recieve(){
string* Connection::Recieve(){
	cout << "Recieving from fd: " << this->fd_other_end << endl;

	unsigned int size_recieved;
	size_recieved = recv(this->fd_other_end, this->recieve_buffer, BUFFER_SIZE, 0 );

	string* res;
	if (size_recieved != strnlen(this->recieve_buffer, BUFFER_SIZE)){
		cout << "Error in recieving" << endl;
		res = NULL;
	} else{
		res = new string(this->recieve_buffer);
		cout << "Recieved: " << this->recieve_buffer << endl;
	}

	return res;
}


void Connection::Close(){

	//this->open_lock->lock();
	pthread_mutex_lock( this->open_lock );
	cout << "Closing file descriptor: " << this->fd_other_end << endl;
	close(this->fd_other_end);
	this->open = false;
	//this->open_lock->unlock();
	pthread_mutex_unlock( this->open_lock );

	//pthread_join(&this->recieve_handler_thread, NULL);
}



bool Connection::is_open() const{
	return this->open;
}
