#include "listeningSocket.hpp"

ListeningSocket::ListeningSocket(char *port)
{
	this->_server_address.sin_family = AF_INET;
	this->_server_address.sin_port = htons(std::atoi(port));
	this->_server_address.sin_addr.s_addr = htonl(INADDR_ANY);

	if ((this->_socket_fd = socket(this->_server_address.sin_family, SOCK_STREAM, 0)) < 0)
	{
		throw(std::runtime_error("Error: socket_fd creation failed"));
	}

	if (bind(this->_socket_fd, (struct sockaddr *)&this->_server_address, sizeof(this->_server_address)) < 0)
	{
		close(this->_socket_fd);
		throw(std::runtime_error("Error: socket bind error"));
	}

	if (listen(this->_socket_fd, 256) < 0)
	{
		close(this->_socket_fd);
		throw(std::runtime_error("Error: socket listen error"));
	}
}
ListeningSocket::~ListeningSocket()
{
	if (_socket_fd >= 0)
	{
		close(this->_socket_fd);
	}
}

int ListeningSocket::getSocket_fd() const
{
	return (this->_socket_fd);
}
