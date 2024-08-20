#pragma once

#include "include.hpp"

class ListeningSocket
{
private:
	struct sockaddr_in	_server_address;
	int					_socket_fd;

public:
	ListeningSocket(char *port);
	~ListeningSocket();

	int	getSocket_fd() const;
};