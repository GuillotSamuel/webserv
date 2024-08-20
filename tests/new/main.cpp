#include "include.hpp"

int main(int argc, char *argv[], char *envp[])
{
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
