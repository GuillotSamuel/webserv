#pragma once

#include "server.hpp"
#include "serverExeution.hpp"

class ListeningSocket
{
    private : 
        int     sockfd;
        struct  sockaddr_in servaddr;
        char    *port;
    public :
        ListeningSocket(char *port);
        ~ListeningSocket;

        //methods


		//getters
		int	getSockFd();
}

ListeningSocket::ListeningSocket(char *port): port(port)
{
    if ((this->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		ft_error("socket creation failed", -1);

    memset(&(this->servaddr), 0, sizeof(this->servaddr));

	this->servaddr.sin_family = AF_INET;
	this->servaddr.sin_port = htons(std::atoi(this->port));
	this->servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(socket_s.sockfd, (SERVADDR_INFO *)&socket_s.servaddr, sizeof(socket_s.servaddr)) < 0)
		ft_error("bind error", -1);
	if (listen(socket_s.sockfd, 10) < 0)
		ft_error("listen error", -1);
}

ListeningSocket::~ListeningSocket()
{
    if (sockfd != -1)
    {
        close(sockfd);
    }
}

ListeningSocket::getSockFd()
{
	return (this->sockfd);
}
