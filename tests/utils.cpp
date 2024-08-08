#include "server.hpp"

void ft_error(std::string str, int connfd)
{
	if (connfd != -1)
		close(connfd);
	std::cerr << "Error: " << str << std::endl;
	exit(EXIT_FAILURE);
}