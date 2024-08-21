#include "Client.hpp"

Client::Client()
{
	
}

Client::~Client()
{

}

std::string Client::getMethod() const
{
	return (this->_method);
}

std::string Client::getIpAdress() const
{
	return (this->_ipAdress);
}
