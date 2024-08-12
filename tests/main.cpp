#include "include.hpp"

int main(int argc, char **argv)
{
	try
	{
		Server server_object(argv[1]);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}

	return (0);
}
