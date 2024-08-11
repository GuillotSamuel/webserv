#pragma once

#include "server.hpp"
#include "serverExecution.hpp"

class ListeningSocket
{
    private : 
        int                 sockfd;
        struct  sockaddr_in servaddr;
        char                *port;
        
    public :
        ListeningSocket(char *port);
        ~ListeningSocket();

        //methods


		//getters
		int	getSockFd() const;
};
