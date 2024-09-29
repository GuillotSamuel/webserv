/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 22:12:59 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/09/29 15:03:56 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Response.hpp"

/*----------------------------------------------------------------------------*/
/*                               CONSTRUCTOR                                  */
/*----------------------------------------------------------------------------*/

Response::Response(Client *client)
{
	this->_serverName = "";
	this->_locationType = "";
	this->_alias = "";
	this->_root = "";
	this->_clientMaxBodySize = 0;
	this->_autoIndex = -1;
	this->_index = "";
	this->_blockName = "";
	this->_client = client;
}

Response::~Response()
{
	this->_interpreterMap.clear();
	this->_allowed_methods.clear();
	this->_errorPages.clear();
	this->_redirection.clear();
}

/*----------------------------------------------------------------------------*/
/*                              METHOD/SERVER                                 */
/*----------------------------------------------------------------------------*/

void	Response::setInfo(ServerConfiguration *serv, Location *location)
{
	this->_serverName = *serv->getServerName().begin();
	if (location)
	{
		this->_locationType = location->getBlockType();
		this->_blockName = location->getBlockName();
		std::cout << location->getAlias();
		this->_alias = location->getAlias();
		
		if (location->getRoot() != "")
			this->_root = location->getRoot();
		else
			this->_root = serv->getRoot();
			
		if (location->getIndex() != "")
			this->_index = location->getIndex();
		else
			this->_index= serv->getIndex();
			
		if (location->getClientMaxBodySize() != -1)
			this->_clientMaxBodySize = location->getClientMaxBodySize();
		else
			this->_clientMaxBodySize = serv->getClientMaxBodySize();
			
		if (!location->getCgi().empty())
			this->_interpreterMap = location->getCgi();
		else
			this->_interpreterMap = serv->getInterpreterMap();
			
		if (location->getAllowedMethods("GET") != -1
			&& location->getAllowedMethods("POST") != -1)
		{
			this->_allowed_methods["GET"] = location->getAllowedMethods("GET");
			this->_allowed_methods["POST"] = location->getAllowedMethods("POST");
		}
		else
			this->_allowed_methods = serv->getAllowedMethods();
			
		if (!location->getErrorPage().empty())
			this->_errorPages = location->getErrorPage();
		else
			this->_errorPages = serv->getErrorPages();
			
		if (location->getAutoIndex() != -1)
			this->_autoIndex = location->getAutoIndex();
		else
			this->_autoIndex = serv->getAutoIndex();
			
		if (!location->getRedirection().empty())
			this->_redirection = location->getRedirection();
	} else {
		this->_root = serv->getRoot();
		this->_index = serv->getIndex();
		this->_clientMaxBodySize = serv->getClientMaxBodySize();
		this->_interpreterMap = serv->getInterpreterMap();
		this->_allowed_methods = serv->getAllowedMethods();
		this->_errorPages = serv->getErrorPages();
		this->_autoIndex = serv->getAutoIndex();
	}
}

std::string	Response::generateResponse()
{
	// if (this->_allowed_methods[this->_client->getMethod()] == 0)
	// 	return (ft_forbidden());
	if (this->_locationType != "" && this->_locationType == "equal")
	{
		_filePath = this->_root + this->_client->getPath();
	}
	if (this->_alias != "")
	{
		std::cout << _blockName << std::endl;
		std::cout << this->_client->getPath() << std::endl;
		size_t stBlock = this->_client->getPath().find(this->_blockName);
		std::cout << this->_alias <<  stBlock << " et " << this->_blockName.size() << std::endl;
		
		this->_client->getPath().replace(stBlock, this->_blockName.size(), this->_alias);
		std::cout << this->_client->getPath() << std::endl;
		_filePath = this->_root + this->_client->getPath();
		std::cout << _filePath << std::endl;
	}
	else if (!this->_redirection.empty())
	{
		_filePath = this->_redirection.begin()->second;
	}
	else 
	{
		_filePath = this->_root + this->_client->getPath();
	}

	// std::cout << _filePath << std::endl; // TEST
	if (access(_filePath.c_str(), F_OK | X_OK) == 0) // trouver le fichier en question si tu trouve pas le fichier
	{
		if (!this->_interpreterMap.empty())//comparer avec l'extension 
		{
			size_t ext = this->_filePath.rfind(".");
			std::string extension = this->_filePath.substr(ext);
			std::map<std::string, std::string>::iterator it = this->_interpreterMap.begin();
			for (; it != this->_interpreterMap.end(); it++)
			{
				if (extension == it->first)
				{
					std::cout << "c'est un cgi" << std::endl;
				}
			}
		}
	} else {
		return (ft_badRequest()); // not found
	}

	
	
	// -> la methods requested -> content length ?
	/*2
	->regarder si cest une redirection 
	->renvoye un status 3XX
	*/

	/* 3
	->regarder si c'est un cgi
	->executer CGI en lui donnant toutes les infos interessante
	*/

	/* 4
	->servir un fichier static
	->If the request is for a static file, find the file on disk using 
	the combination of _root and _path.
	If the file exists, set the appropriate headers (e.g., Content-Type 
	based on the file extension) and send the file content
	->Use _index to find a default file if the request is for a directory.
	*/

	/* 5
	->If any error occurs (e.g., file not found, method not allowed),
	serve an appropriate error page from _errorPages.
	*/

	/*	6. Headers

    ->Add necessary response headers based on the client’s request
	(e.g., Content-Type, Content-Length, Connection).
    ->Ensure Content-Length is calculated for the body being sent.
    ->Handle Accept-Language (_httpAcceptLanguage) and User-Agent
	(_httpUserAgent) if you want to customize the response based on them.

	7. Autoindex
    ->If _autoIndex is enabled for a directory, generate an index page listing the files and directories if no _index file is found.

	8. Send Response
    ->Compose the response with the proper HTTP status code
	(e.g., 200 for success, 404 for not found), headers, and the body
	content (file content, CGI output, or error page).
    ->Write the response to _currentFd, ensuring the socket is ready
	for writing.*/
	
	if (this->_client->getMethod() == "GET")
		return (ft_get());
	else if (this->_client->getMethod() == "POST")
		return (ft_post());
	else if (this->_client->getMethod() == "DELETE")
		return (ft_delete());
	else
		return (ft_badRequest());
}

std::string Response::ft_get() // a revoir
{
	Server::log("Server's receive a GET request.", 1);
	
	std::string content = readFileContent(this->_filePath);
	std::string response = "";
	std::stringstream ss;
	// if (this->_status_code != 0)
	// {
	// 	Server::log("The file requested \"" + getFilePath() + "\" was found empty.", 1);
	// 	content = readFileContent(getFilePath());

	// 	response = "HTTP/1.1 200 Bad Request\r\n"; /* + _code + */
	// 	response += "Content-Type: text/html\r\n";
	// 	std::ostringstream oss;
	// 	oss << content.size();
	// 	response += "Content-Length: " + oss.str() + "\r\n";
	// 	response += "Connection: close\r\n";
	// 	response += "Server: webserv/1.0\r\n\r\n";
	// 	response += content;
	// }
	// else
	// {
		Server::log("The file requested \"" + this->_filePath + "\" was found.", 1);
		std::string mimeType = getMimeType();

		response = "HTTP/1.1 200 OK\r\n";
		response += "Content-Type: " + mimeType + "\r\n";
		std::ostringstream oss;
		oss << content.size();
		response += "Content-Length: " + oss.str() + "\r\n";
		response += "Connection: close\r\n";
		response += "Server: " + this->_serverName + "\r\n\r\n";
		response += content;
	// }
	return (response);
}

/*response to a POST request*/
std::string Response::ft_post()
{
	Server::log("Server's receive a POST request.", 1);
	std::string content = readFileContent(this->_filePath);
	std::string response = "";

	Server::log("The file requested \"" + this->_filePath + "\" was found.", 1);
	std::string mimeType = getMimeType();

	response = "HTTP/1.1 200 OK\r\n";
	response += "Content-Type: " + mimeType + "\r\n";
	std::ostringstream oss;
	oss << content.size();
	response += "Content-Length: " + oss.str() + "\r\n";
	response += "Connection: close\r\n";
	response += "Server: " + this->_serverName + "\r\n\r\n";
	response += content;

	return (response);
}

/*response to a DELETE request*/
std::string Response::ft_delete()
{
	Server::log("Server's receive a DELETE request.", 1);

	if (access(getFilePath().c_str(), F_OK) != 0)
	{
		std::string response = "HTTP/1.1 404 Not Found\r\n";
		response += "Content-Type: text/html\r\n";
		std::string content = "File not found";
		std::ostringstream oss;
		oss << content.size();
		response += "Content-Length: " + oss.str() + "\r\n";
		response += "Connection: close\r\n";
		response += "Server: " + this->_serverName + "\r\n\r\n";
		response += "File not found";

		return (response);
	}

	if (unlink(getFilePath().c_str()) == 0)
	{
		std::string response = "HTTP/1.1 204 No Content\r\n";
		response += "Connection: close\r\n";
		response += "Server: " + this->_serverName + "\r\n\r\n";

		return (response);
	}
	else
		Server::log("Server failed to respond at the DELETE request.", 2);
	return ("");
}

/*response to a bad request*/
std::string Response::ft_badRequest()
{
	std::string content;
	if (this->_errorPages.find(404) != this->_errorPages.end()) {
		content = readFileContent(this->_root + this->_errorPages.find(404)->second);
	} else {
		content = readFileContent("/home/user/ecole_42/webserv/webserv_v1.0/www/error_pages/404.html");	
	}

	std::string response = "HTTP/1.1 400 Bad Request\r\n";
	response += "Content-Type: text/html\r\n";
	std::ostringstream oss;
    oss << content.size();
    response += "Content-Length: " + oss.str() + "\r\n";
	response += "Connection: close\r\n";
	response += "Server: " + this->_serverName + "\r\n\r\n";
	response += content;
	
	return (response);
}

std::string Response::ft_forbidden()
{
	std::string content = readFileContent("");

	std::string response = "HTTP/1.1 400 Bad Request\r\n";
	response += "Content-Type: text/html\r\n";
	std::ostringstream oss;
    oss << content.size();
    response += "Content-Length: " + oss.str() + "\r\n";
	response += "Connection: close\r\n";
	response += "Server: " + this->_serverName + "\r\n\r\n";
	response += content;
	
	return (response);
}

std::string	Response::readFileContent(std::string path)
{
	std::ifstream file(path.c_str());

	if (!file.is_open())
	{
		return ("");
	}

	std::ostringstream oss;
	oss << file.rdbuf();
	return (oss.str());
}

std::string	Response::getMimeType()
{
	if (getFilePath() == "/" )
		return ("text/html");

	return ("application/octet-stream");
}

/*----------------------------------------------------------------------------*/
/*                                   SETTER                                   */
/*----------------------------------------------------------------------------*/

void Response::setFilePath(std::string root, std::string fileRequested)
{
	this->_filePath = root + fileRequested;
}

/*----------------------------------------------------------------------------*/
/*                                   GETTER                                   */
/*----------------------------------------------------------------------------*/

std::string Response::getServerName() const
{
	return (this->_serverName);
}

std::string	Response::getAlias() const
{
	return (this->_alias);
}

std::string	Response::getRoot() const
{
	return (this->_root);
}

std::string	Response::getIndex() const
{
	return (this->_index);
}

int	Response::getClientMaxBodySize() const
{
	return (this->_clientMaxBodySize);
}

std::map<std::string, std::string>	Response::getCgi() const
{
	return (this->_interpreterMap);
}

// int	Response::getAllowedMethods(std::string wichOne)const
// {
// 	//?
// }

std::map<std::string, int>	Response::getAllowedMethodsTab() const
{
	return (this->_allowed_methods);
}

int	Response::getAutoIndex() const
{
	return (this->_autoIndex);
}

std::string Response::getFilePath() const
{
	return (this->_filePath);
}

std::map<int, std::string>	Response::getErrorPage() const
{
	return (this->_errorPages);
}

std::map<int, std::string>	Response::getRedirection() const
{
	return (this->_redirection);
}

/*----------------------------------------------------------------------------*/
/*                                  UTILS                                     */
/*----------------------------------------------------------------------------*/

std::ostream &operator<<(std::ostream &Cout, Response const &response)
{
	Cout << "server name	:" << response.getServerName() << std::endl;
	Cout << "alias			:" << response.getAlias() << std::endl;
	Cout << "root			:" << response.getRoot() << std::endl;
	Cout << "index			:" << response.getIndex() << std::endl;
	Cout << "Clientbody		:" << response.getClientMaxBodySize() << std::endl;
	Cout << "Autoindex		:" << response.getAutoIndex() << std::endl;
	Cout << "Cgi map		:" << std::endl;
	std::map<std::string, std::string> pathinfo = response.getCgi();
	std::map<std::string, std::string>::iterator it1 = pathinfo.begin();
	for (; it1 != pathinfo.end(); it1++)
	{
		Cout << it1->first << " | " << it1->second << std::endl;
	}
	Cout << "Allow map		: " << std::endl;
	std::map<std::string, int> allowedmet = response.getAllowedMethodsTab();
	std::map<std::string, int>::iterator it2 = allowedmet.begin();
	for (; it2 != allowedmet.end(); it2++)
	{
		Cout << it2->first << " | " << it2->second << std::endl;
	}
	Cout << "Error map		: " << std::endl;
	std::map<int, std::string> error = response.getErrorPage();
	std::map<int, std::string>::iterator it3 = error.begin();
	for (; it3 != error.end(); it3++)
	{
		Cout << it3->first << " | " << it3->second << std::endl;
	}
	// Cout << "redirection map		: " << std::endl;
	// std::map<int, std::string> redir = response.getRedirection();
	// std::map<int, std::string>::iterator it4 = redir.begin();
	// for (; it4 != error.end(); it4++)
	// {
	// 	Cout << it4->first << " | " << it4->second << std::endl;
	// }
	return (Cout);
}
