#ifndef CONNECTIONS_INCLUDES_HPP
#define CONNECTIONS_INCLUDES_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

#define PORT 9999
#define BUFFER_SIZE 1024

typedef int fd_t; //File descriptor type

#endif