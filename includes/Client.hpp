#ifndef	CLIENT_HPP
#define CLIENT_HPP
#include "webserv.hpp"

class Client
{
	private:
		std::string _method;
		std::string	_ipAdress;


	public:
		Client();
		~Client();
		std::string getMethod() const;
		std::string getIpAdress() const;
};

#endif