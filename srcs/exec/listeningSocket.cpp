#include "listeningSocket.hpp"

ListeningSocket::ListeningSocket(int port)
{
	socklen_t address_len = sizeof(this->_server_address);
	this->_server_address.sin_family = AF_INET;
	this->_server_address.sin_port = htons(port);
	this->_server_address.sin_addr.s_addr = htonl(INADDR_ANY);

	this->_proto = getprotobyname("tcp");
	if (this->_proto == NULL)
	{
		std::cout << "ERROR : getprotobyname" << std::endl; // TEST
	}

	if ((this->_socket_fd = socket(this->_server_address.sin_family, SOCK_STREAM, this->_proto->p_proto)) < 0)
	{
		throw(std::runtime_error("Error: socket_fd creation failed"));
	}

	if (setsockopt(this->_socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &this->_proto, sizeof(this->_proto))) {
        close(this->_socket_fd);
		std::cout << "can't reuse port " << std::endl; // TEST
    }

	if (bind(this->_socket_fd, (struct sockaddr *)&this->_server_address, sizeof(this->_server_address)) < 0)
	{
		close(this->_socket_fd);
		throw(std::runtime_error("Error: socket bind error"));
	}

	if (getsockname(this->_socket_fd, (struct sockaddr *)&this->_server_address, &address_len) == -1)
	{
		std::cout << "getsocketname" << std::endl; // TEST
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
