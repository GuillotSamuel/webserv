#pragma once

#include "webserv.hpp"

class ListeningSocket
{
	private:
		struct sockaddr_in	_server_address;
		int					_socket_fd;
		struct protoent		*_proto;
		ServerConfiguration	&_serv;
		

	public:
		ListeningSocket(int port, ServerConfiguration &serv);
		~ListeningSocket();

		int		getSocket_fd() const;
		void	set_nonblocking(int sockfd);
		bool	operator==(const ListeningSocket& other) const;
		bool	operator<(const ListeningSocket& other) const;
};