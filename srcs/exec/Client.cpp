#include "Client.hpp"

/*------------------------------------CONSTRUCTOR----------------------------------------*/

Client::Client()
{
	this->_httpAcceptLanguage = "";
	this->_httpAcceptMime = "";
	this->_httpReferer = "";
	this->_httpUserAgent = "";
	this->_ipAdress = "";
	this->_method = "";
}

Client::~Client()
{

}

/*------------------------------------METHOD----------------------------------------*/

void Client::setInfo(std::string info)
{
	size_t methodEnd = info.find(" ");
	if (methodEnd == std::string::npos)
		std::cout << "Method Failed!" << std::endl; // TEST
	else
		this->setMethod(info.substr(0, methodEnd));

	size_t userAgent = info.find("User-Agent: ") + 12;
	if (userAgent == std::string::npos)
		std::cout << "UserAgent Failed!" << std::endl; // TEST
	else
	{
		size_t endUserAgent = info.find("\r\n", userAgent);
		if (endUserAgent == std::string::npos)
			std::cout << "EndUserAgent Failed!" << std::endl; // TEST
		else
			this->setUserAgent(info.substr(userAgent, (endUserAgent - userAgent)));
	}

	size_t accept = info.find("Accept: ") + 9;
	if (accept == std::string::npos)
		std::cout << "Accept Failed!" << std::endl; // TEST
	else
	{
		size_t	endAccept = info.find("\r\n", accept);
		if (endAccept == std::string::npos)
			std::cout << "EndAccept Failed!" << std::endl; // TEST
		else
			this->setAcceptMime(info.substr(accept, (endAccept - accept)));
	}

	size_t acceptLanguage = info.find("Accept-Language: ") + 17;
	if (acceptLanguage == std::string::npos)
		std::cout << "acceptLanguage Failed!" << std::endl; // TEST
	else
	{
		size_t endAcceptLanguage = info.find("\r\n", acceptLanguage);
		if (endAcceptLanguage == std::string::npos)
			std::cout << "endAcceptLanguage Failed!" << std::endl; // TEST
		else
			this->setAcceptLanguage(info.substr(acceptLanguage, (acceptLanguage - endAcceptLanguage)));
	}

	size_t referer = info.find("Referer: ") + 9;
	if (referer == std::string::npos)
		std::cout << "Referer Failed!" << std::endl; // TEST
	else
	{
		size_t endReferer = info.find("\r\n", referer);
		if (endReferer == std::string::npos)
			std::cout << "EndReferer failed!" << std::endl; // TEST
		else
			this->setReferer(info.substr(referer, (endReferer - referer)));
	}
}

/*------------------------------------ACCESSOR----------------------------------------*/

std::string Client::getMethod() const {
	return (this->_method);
}

std::string Client::getIpAdress() const {
	return (this->_ipAdress);
}

void Client::setMethod(std::string method) {
	this->_method = method;
}

void Client::setIpAddress(std::string ipAdress) {
	this->_ipAdress = ipAdress;
}

void Client::setAcceptMime(std::string httpAcceptMime) {
	this->_httpAcceptMime = httpAcceptMime;
}

void Client::setAcceptLanguage(std::string httpAcceptLanguage) {
	this->_httpAcceptLanguage = httpAcceptLanguage;
}

void Client::setUserAgent(std::string userAgent) {
	this->_httpUserAgent = userAgent;
}

void Client::setReferer(std::string referer) {
	this->_httpReferer = referer;
}

/*--------------------------------ERROR MANAGEMENT------------------------------------*/

void		Client::error(std::string errorType)
{
	throw(std::runtime_error(errorType));
}