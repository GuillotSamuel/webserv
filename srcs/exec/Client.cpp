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
	this->_contentLength = "";
	this->_contentType = "";
}

Client::~Client()
{
}

/*------------------------------------METHOD----------------------------------------*/

void Client::setInfo(std::string info)
{
	size_t methodEnd = info.find(" ");
	if (methodEnd != std::string::npos)
		this->setMethod(info.substr(0, methodEnd));

	size_t userAgent = info.find("User-Agent: ");
	if (userAgent != std::string::npos)
	{
		userAgent += 12;
		size_t endUserAgent = info.find("\r\n", userAgent);
		if (endUserAgent != std::string::npos)
			this->setUserAgent(info.substr(userAgent, (endUserAgent - userAgent)));
	}

	size_t accept = info.find("Accept: ");
	if (accept != std::string::npos)
	{
		accept += 8;
		size_t	endAccept = info.find("\r\n", accept);
		if (endAccept != std::string::npos)
			this->setAcceptMime(info.substr(accept, (endAccept - accept)));
	}

	size_t acceptLanguage = info.find("Accept-Language: ");
	if (acceptLanguage != std::string::npos)
	{
		acceptLanguage += 17;
		size_t endAcceptLanguage = info.find("\r\n", acceptLanguage);
		if (endAcceptLanguage != std::string::npos)
			this->setAcceptLanguage(info.substr(acceptLanguage, (endAcceptLanguage - acceptLanguage)));
	}

	size_t referer = info.find("Referer: ");
	if (referer != std::string::npos)
	{
		referer += 9;
		size_t endReferer = info.find("\r\n", referer);
		if (endReferer != std::string::npos)
			this->setReferer(info.substr(referer, (endReferer - referer)));
	}

	size_t contentType = info.find("Content-Type: ");
	if (contentType != std::string::npos)
	{
		contentType += 14;
		size_t endContentType = info.find(";", contentType);
		if (endContentType != std::string::npos)
			this->setContentType(info.substr(contentType, (endContentType - contentType)));
	}

	size_t contentLength = info.find("Content-Length: ");
	if (contentLength != std::string::npos)
	{
		contentLength += 16;
		size_t endContentLength = info.find("\r\n", contentLength);
		if (endContentLength != std::string::npos)
			this->setContentLength(info.substr(contentLength, (endContentLength - contentLength)));
	}
}

/*------------------------------------ACCESSOR----------------------------------------*/

std::string	Client::getMethod() const {
	return (this->_method);
}

std::string	Client::getIpAdress() const {
	return (this->_ipAdress);
}

std::string	Client::getAcceptMime() const{
	return (this->_httpAcceptMime);
}

std::string	Client::getAcceptLanguage() const {
	return (this->_httpAcceptLanguage);
}

std::string	Client::getUserAgent() const {
	return (this->_httpUserAgent);
}
std::string	Client::getReferer() const {
	return (this->_httpReferer);
}

std::string	Client::getContentType() const {
	return (this->_contentType);
}

std::string	Client::getContentLength() const {
	return (this->_contentLength);
}

void	Client::setMethod(std::string method) {
	this->_method = method;
}

void	Client::setIpAddress(std::string ipAdress) {
	this->_ipAdress = ipAdress;
}

void	Client::setAcceptMime(std::string httpAcceptMime) {
	this->_httpAcceptMime = httpAcceptMime;
}

void	Client::setAcceptLanguage(std::string httpAcceptLanguage) {
	this->_httpAcceptLanguage = httpAcceptLanguage;
}

void	Client::setUserAgent(std::string userAgent) {
	this->_httpUserAgent = userAgent;
}

void	Client::setReferer(std::string referer) {
	this->_httpReferer = referer;
}

void	Client::setContentType(std::string contentType) {
	this->_contentType = contentType;
}

void	Client::setContentLength(std::string contentLength) {
	this->_contentLength = contentLength;
}

/*--------------------------------ERROR MANAGEMENT------------------------------------*/

void		Client::error(std::string errorType)
{
	throw(std::runtime_error(errorType));
}

std::ostream &operator<<(std::ostream &Cout, Client const &i)
{
	Cout << "info client	: " << std::endl;
	Cout << "Method		: " << i.getMethod() << std::endl;
	Cout << "Ip adress	: " << i.getIpAdress() << std::endl;
	Cout << "User-Agent	: " << i.getUserAgent() << std::endl;
	Cout << "Referer		: " << i.getReferer() << std::endl;
	Cout << "language	: " << i.getAcceptLanguage() << std::endl;
	Cout << "Mime		: " << i.getAcceptMime() << std::endl;
	Cout << "Content-Length : " << i.getContentLength() << std::endl;
	Cout << "Content-Type	: " << i.getContentType() << std::endl;
	
	return (Cout);
}
