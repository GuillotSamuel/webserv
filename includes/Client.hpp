#ifndef	CLIENT_HPP
#define CLIENT_HPP
#include "webserv.hpp"

class Client
{
	private:
		std::string _method;
		std::string	_ipAdress;
		std::string _httpAcceptMime; // text/*, image/jpeg, image/png, image/*, */*
		std::string _httpAcceptLanguage; // fr_CA, fr
		std::string _httpUserAgent; // la naviguateur utilise par le client // Mozilla/5.0 (compatible; Konqueror/3; Linux)
		std::string	_httpReferer; //Une adresse absolue ou partielle de la page web à partir de laquelle la requête vers la page courante a été émise.


	public:
		Client();
		~Client();
		std::string getMethod() const;
		std::string getIpAdress() const;

		void	setInfo(std::string info);
		void	setMethod(std::string method);
		void	setIpAddress(std::string ipAdress);
		void	setAcceptMime(std::string httpAcceptMime);
		void	setAcceptLanguage(std::string httpAcceptLanguage);
		void	setUserAgent(std::string userAgent);
		void	setReferer(std::string referer);
		void	error(std::string errorType);
};


#endif