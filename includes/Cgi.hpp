#ifndef CGI_HPP
#define CGI_HPP

#include "webserv.hpp"

class Client;

class Cgi
{
	private:
		std::map<std::string, std::string>	_env;
		char								**_myEnvp;
		const char							*_path;
		char								**_myArgv;
		std::string							_pathInfo;

	public:
		Cgi();
		~Cgi();
		std::string		executeCgi();
		char			**conversionEnvFunc();
		char			**createArgv();
		void			setEnv(ServerConfiguration server, Client client);
		void			setPath(const char *path);
		void			error(std::string errorType);
		std::string		searchPathInfo();
};

#endif