#include "server.hpp"

void ft_error(std::string str)
{
	std::cerr << "Error: " << str << std::endl;
	exit(EXIT_FAILURE);
}