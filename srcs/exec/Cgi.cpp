#include "Cgi.hpp"

Cgi::Cgi(ServerConfiguration serv, Client client)
{
	if (client.getMethod() == "POST")
	{
		this->_env["CONTENT_TYPE"] = std::string(""); // only for post
		this->_env["CONTENT_LENGTH"] = std::string(""); // only for post
	}

	//SERVEUR_VAR
	this->_env["SERVER_SOFTWARE"] = std::string("nom et version du serveur");
	this->_env["SERVER_NAME"] = serv.getServerName();
	this->_env["GATEWAY_INTERFACE"] = std::string("CGI/1.1");

	//REQUEST_VAR
	this->_env["SERVER_PROTOCOL"] = std::string("TCP/IP");
	this->_env["SERVER_PORT"] = serv.getPort();
	this->_env["REQUEST_METHOD"] = client.getMethod();
	// this->_env["PATH_INFO"] = serv.getPathInfo();
	this->_env["PATH_TRANSLATED"] = (std::string(""));
	this->_env["SCRIPT_NAME"] = std::string(""); //le chemin vers le script executer
	this->_env["QUERY_STRING"] = std::string("");
	this->_env["REMOTE_HOST"] = std::string(""); // laisse vide si inconnu
	this->_env["REMOTE_ADDR"] = client.getIpAdress();
	// this->_env["AUTH_TYPE"] = ;
	// this->_env["REMOTE_USER"] = ;
	// this->_env["REMOTE_IDENT"] = ;

	//CLIENT_VAR
	this->_env["HTTP_ACCEPT"] = std::string("");
	this->_env["HTTP_ACCEPT_LANGUAGE"] = std::string("");
	this->_env["HHTP_USER_AGENT"] = std::string("");
	this->_env["HTTP_COOKIE"] = std::string("");
	this->_env["HTTP_REFERER"] = std::string("");
}

Cgi::~Cgi()
{
}
