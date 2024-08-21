#pragma once

#include "webserv.hpp"

class ListeningSocket
{
	private:
		struct sockaddr_in	_server_address;
		int					_socket_fd;

	public:
		ListeningSocket(int port);
		~ListeningSocket();

		int	getSocket_fd() const;
};