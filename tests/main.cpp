#include "server.hpp"
#include "serverExecution.hpp"
#include "socket.hpp"

int main(int argc, char **argv)
{
    if (argc != 2)
		ft_error("wrong arguments");

    try
    {
        ListeningSocket socket1(argv[1]);

        ServerExecution server(socket);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return (0);
}
