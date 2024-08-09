#include "server.hpp"

int main(int argc, char **argv)
{
    if (argc != 2)
		ft_error("wrong arguments");

    try
    {
        // ListeningSocket *c_socket = new ListeningSocket(argv[1]);

        ServerExecution c_server(argv[1]);

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return (0);
}
