#include "listeningSocket.hpp"

ListeningSocket::ListeningSocket(char *port): port(port)
{
    if ((this->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		ft_error("socket creation failed");

    memset(&(this->servaddr), 0, sizeof(this->servaddr));

	this->servaddr.sin_family = AF_INET;
	this->servaddr.sin_port = htons(std::atoi(this->port));
	this->servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(this->sockfd, (SERVADDR_INFO *)&this->servaddr, sizeof(this->servaddr)) < 0)
		ft_error("bind error");
	if (listen(this->sockfd, 10) < 0)
		ft_error("listen error");
}

ListeningSocket::~ListeningSocket()
{
    if (sockfd != -1)
    {
        close(sockfd);
    }
}

int ListeningSocket::getSockFd() const
{
	return (this->sockfd);
}

// void	ListeningSocket::setSockfd()
// {

// }

// void	ListeningSocket::setServaddr()
// {

// }

// void	ListeningSocket::setPort(char *port)
// {

// }