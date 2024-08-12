#pragma once

#include "listeningSocket.hpp"

class ListeningSocket;

class Server
{
private:
	ListeningSocket 	*_socket;
	struct epoll_event	_event;
	struct epoll_event	_events[MAX_EVENTS];
	int					_epoll_fd;
	int					_connexion_fd;
	std::string			_method;
	std::string			_path;
	char				received_line[BUFFER_SIZE];
	char				socket_buffer[BUFFER_SIZE];

	void				ServerExecution();
	std::string			findContent(const std::string &receivedLine);
	std::string			findMethod(const std::string &receivedLine);
	void				handle_client();
	std::string			getFileContent(const std::string &path);

public:
	Server(char *port);
	~Server();
};
