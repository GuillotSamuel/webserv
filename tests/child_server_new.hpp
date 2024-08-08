#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/epoll.h>

#define SERVADDR_INFO struct sockaddr
#define MAX_EVENTS 10
#define BUFFER_SIZE 4096

typedef struct s_socket
{
	int sockfd;
	int connfd;
	char *port;
	struct sockaddr_in servaddr;
	char receiveline[BUFFER_SIZE];
	char socket_buffer[BUFFER_SIZE];
	int n;
	std::string path;
}	t_socket;

