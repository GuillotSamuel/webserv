#pragma once

#include "webserv.hpp"

class ListeningSocket
{
	private:
		struct sockaddr_in	_server_address;
		int					_socket_fd;
		struct protoent		*_proto;

	public:
		ListeningSocket(int port);
		~ListeningSocket();

		int	getSocket_fd() const;
		void set_nonblocking(int sockfd);
};