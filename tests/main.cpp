#include "include.hpp"

int main(int argc, char *argv[], char *envp[])
{
	// std::list<FormData> my_list;
	// std::list<FormData>::iterator it;
	try
	{
		Server server_object(argv[1], envp);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}

	return (0);
}
