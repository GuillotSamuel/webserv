#include "listeningSocket.hpp"

/*----------------------------------------------------------------------------*/
/*                               CONSTRUCTOR                                  */
/*----------------------------------------------------------------------------*/

ListeningSocket::ListeningSocket(int port)
{
	socklen_t address_len = sizeof(this->_server_address);
	this->_server_address.sin_family = AF_INET;
	this->_server_address.sin_port = htons(port);
	this->_server_address.sin_addr.s_addr = htonl(INADDR_ANY);

	this->_proto = getprotobyname("tcp");
	if (this->_proto == NULL)
	{
		Server::log("Getprotobyname didn't worked for some reason.", 2); 
	}

	Server::log("Getprotobyname Succed.", 1);

	if ((this->_socket_fd = socket(this->_server_address.sin_family, SOCK_STREAM, this->_proto->p_proto)) < 0)
		Server::log("Socket_fd creation failed.", 2);

	Server::log("Socket_fd in now created.", 1);

	if (setsockopt(this->_socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &this->_proto, sizeof(this->_proto))) {
        close(this->_socket_fd);
		Server::log("Port was not reuse.", 2);
    }

	Server::log("Socket_fd in now set.", 1);

	if (bind(this->_socket_fd, (struct sockaddr *)&this->_server_address, sizeof(this->_server_address)) < 0)
	{
		close(this->_socket_fd);
		Server::log("Socket bind error.", 2);
	}

	Server::log("Socket_fd is binded.", 1);

	if (getsockname(this->_socket_fd, (struct sockaddr *)&this->_server_address, &address_len) == -1)
	{
		Server::log("Getsocketname Failed.", 2);
	}

	set_nonblocking(this->_socket_fd);
	
	Server::log("Socket_fd is non blocking fd.", 1);

	if (listen(this->_socket_fd, 256) < 0)
	{
		close(this->_socket_fd);
		Server::log("Listen Failed.", 2);
	}

	Server::log("Socket_fd is now listening every attemp of connection.", 1);
}
ListeningSocket::~ListeningSocket()
{
	if (_socket_fd >= 0)
	{
		close(this->_socket_fd);
	}
}

/*----------------------------------------------------------------------------*/
/*                                  UTILS                                     */
/*----------------------------------------------------------------------------*/

int ListeningSocket::getSocket_fd() const
{
	return (this->_socket_fd);
}

void ListeningSocket::set_nonblocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        exit(EXIT_FAILURE);
    }

    flags |= O_NONBLOCK;
    if (fcntl(sockfd, F_SETFL, flags) == -1) {
        perror("fcntl F_SETFL");
        exit(EXIT_FAILURE);
    }
}

bool	ListeningSocket::operator==(const ListeningSocket& other) const {    
	return (other.getSocket_fd() == this->getSocket_fd());
}

bool	ListeningSocket::operator<(const ListeningSocket& other) const {    
	return (other.getSocket_fd() < this->getSocket_fd());
}