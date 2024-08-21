#ifndef CGI_HPP
#define CGI_HPP

#include "webserv.hpp"

// class ServerConfiguration;
class Client;

class Cgi
{
	private:
		std::map<std::string, std::string>	_env;
	public:
		Cgi(ServerConfiguration serv, Client client);
		~Cgi();
};

#endif