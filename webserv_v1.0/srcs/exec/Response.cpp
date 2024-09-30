/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <mmahfoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 22:12:59 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/09/30 19:08:51 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Response.hpp"

/*----------------------------------------------------------------------------*/
/*                               CONSTRUCTOR                                  */
/*----------------------------------------------------------------------------*/

Response::Response(Client *client)
{
	this->_locationType = "";
	this->_alias = "";
	this->_root = "";
	this->_clientMaxBodySize = 0;
	this->_autoIndex = -1;
	this->_index = "";
	this->_blockName = "";
	this->_client = client;
	this->_mimePath = createMimePath();
	this->_code = "";
	this->_autoIndexUse = 0;
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

void	Response::setInfo(ServerConfiguration *serv, Location location)
{
	std::vector<std::string> tab = serv->getServerName();
	std::vector<std::string>::iterator it = tab.begin();
	for (; it != tab.end(); it++)
	{
		this->_serverName.push_back(*it);
	}
	if (location.getBlockName() != "")
	{
		this->_locationType = location.getBlockType();
		this->_blockName = location.getBlockName();
		this->_alias = location.getAlias();
		
		if (location.getRoot() != "")
			this->_root = location.getRoot();
		else
			this->_root = serv->getRoot();
			
		if (location.getIndex() != "")
			this->_index = location.getIndex();
		else
			this->_index= serv->getIndex();
			
		if (location.getClientMaxBodySize() != -1)
			this->_clientMaxBodySize = location.getClientMaxBodySize();
		else
			this->_clientMaxBodySize = serv->getClientMaxBodySize();
			
		if (!location.getCgi().empty())
			this->_interpreterMap = location.getCgi();
		else
			this->_interpreterMap = serv->getInterpreterMap();
			
		if (!location.getAllowedMethodsTab().empty())
		{
			this->_allowed_methods["GET"] = location.getAllowedMethods("GET");
			this->_allowed_methods["POST"] = location.getAllowedMethods("POST");
		}
		else
			this->_allowed_methods = serv->getAllowedMethods();
			
		if (!location.getErrorPage().empty())
			this->_errorPages = location.getErrorPage();
		else
			this->_errorPages = serv->getErrorPages();
			
		if (location.getAutoIndex() != -1)
			this->_autoIndex = location.getAutoIndex();
		else
			this->_autoIndex = serv->getAutoIndex();
			
		if (!location.getRedirection().empty())
			this->_redirection = location.getRedirection();
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
	if (this->_allowed_methods[this->_client->getMethod()] != 1)
	{
		_code = "403";
		return (ft_forbidden());
	}
	filePathFinder();
	if (_autoIndexUse == 1)
		return (autoIndex());
	std::cout << _filePath << std::endl;

	if (access(_filePath.c_str(), F_OK) == 0)
	{
		std::cout << this->_filePath << std::endl;
		if (!this->_interpreterMap.empty())
		{
			
			size_t ext = this->_filePath.rfind(".");
			if (ext != std::string::npos)
			{
				std::string _extension = this->_filePath.substr(ext);
				std::map<std::string, std::string>::iterator it = this->_interpreterMap.begin();
				for (; it != this->_interpreterMap.end(); it++)
				{
					if (_extension == it->first)
					{
						cgiExecution();
					}
				}
			}
		}
	} else {
		_code = "404";
		return (ft_badRequest());
	}
	
	// -> la methods requested -> content length ?
	/*7. Autoindex
    ->If _autoIndex is enabled for a directory, generate an index page listing the files and directories if no _index file is found.*/

	if (this->_client->getMethod() == "GET")
		return (ft_get());
	else if (this->_client->getMethod() == "POST")
		return (ft_post());
	else if (this->_client->getMethod() == "DELETE")
		return (ft_delete());
	else
		return (ft_badRequest());
}

std::string	Response::firstHeader()
{
	if (_code == "301")
		return ("HTTP/1.1 301 Moved Permanently\r\n");
	else if (_code == "302")
		return ("HTTP/1.1 302 Moved Temporarily\r\n");
	return ("HTTP/1.1 200 OK\r\n");
}

std::map<std::string, std::string>	Response::createEnvCgi()
{
	std::map<std::string, std::string> env;
	if (_client->getMethod() == "POST")
	{
		env["CONTENT_TYPE"] = _client->getContentType(); // only for post
		env["CONTENT_LENGTH"] = _client->getContentLength(); // only for post
	}
	//SERVEUR_VAR
	env["SERVER_SOFTWARE"] = std::string("Webserv/1.0");
	env["SERVER_NAME"] = *this->_serverName.begin();
	env["GATEWAY_INTERFACE"] = std::string("CGI/1.1");

	//REQUEST_VAR
	env["SERVER_PROTOCOL"] = std::string("HTTP/1.1");
	env["SERVER_PORT"] = _client->getPortStr();
	env["REQUEST_METHOD"] = _client->getMethod();
	env["PATH_INFO"] = _client->getPath();
	env["PATH_TRANSLATED"] = _filePath;
	env["SCRIPT_NAME"] = _filePath;
	env["QUERY_STRING"] = std::string(""); // ???
	env["REMOTE_HOST"] = std::string("");
	env["REMOTE_ADDR"] = _client->getIpAdress();

	//CLIENT_VAR
	env["HTTP_ACCEPT"] = _client->getAcceptMime();
	env["HTTP_ACCEPT_LANGUAGE"] = _client->getAcceptLanguage();
	env["HTTP_USER_AGENT"] = _client->getUserAgent();
	env["HTTP_COOKIE"] = std::string("");
	env["HTTP_REFERER"] = _client->getReferer();
	return (env);
}

std::string	Response::cgiExecution()
{
	Cgi *cgi = new Cgi();
	// cgi->setExecuter(this->_executer_cgi);
	cgi->setPath(_filePath.c_str());
	cgi->setEnv(createEnvCgi()); 
	std::string content = cgi->executeCgi();

	std::string mimeType = getMimeType();

	std::string response = "HTTP/1.1 200 OK\r\n";
	response += "Content-Type: " + mimeType + "\r\n";
	std::ostringstream oss;
	oss << content.size();
	response += "Content-Length: " + oss.str() + "\r\n";
	response += "Connection: close\r\n";
	response += "Server: " + *this->_serverName.begin() + "\r\n\r\n";
	response += content;

	delete cgi;
	return (response);
}

void	Response::filePathFinder()
{
	if (this->_locationType != "" && this->_locationType == "equal")
	{
		_code = "200";
		_filePath = this->_root + this->_client->getPath();
	}
	else if (this->_alias != "")
	{
		_code = "200";
		size_t stBlock = this->_client->getPath().find(this->_blockName);
		
		std::string path_tmp = this->_client->getPath();
		if (stBlock != std::string::npos)
			path_tmp.replace(stBlock, this->_blockName.size(), this->_alias);
		_filePath = this->_root + path_tmp;
	}
	else if (!this->_redirection.empty() && this->_client->getPath() == this->_blockName)
	{
		std::stringstream ss;
		ss << this->_redirection.begin()->first;
		std::string str = ss.str();
		_code = str;
		_filePath = this->_redirection.begin()->second;
	}
	else if (this->_client->getPath() == "/")
	{
		_code = "200";
		if (_index != "")
			_filePath = this->_root + this->_index;
		else if (_autoIndex == 1)
		{
			_autoIndexUse = 1;
			return ;
		}
		else
			_filePath = "";
	}
	else
	{
		_code = "200";
		if (this->_index != "")
		{
			std::vector<std::string>::iterator it = this->_serverName.begin();
			for (; it != _serverName.end(); it++)
			{
				if (_client->getPath() == "/" + *it)
					_filePath = this->_root + this->_index;
			}
		}
		else if (_autoIndex == 1)
		{
			std::vector<std::string>::iterator it = this->_serverName.begin();
			for (; it != _serverName.end(); it++)
			{
				if (_client->getPath() == "/" + *it)
				{
					_autoIndexUse = 1;
					return ;
				}
			}
		}
		if (_filePath == "")
			_filePath = this->_root + this->_client->getPath();
	}
}

std::string Response::ft_get() // a revoir
{
	Server::log("Server's receive a GET request.", 1);
	std::cout << this->_filePath << std::endl;
	std::string content = readFileContent(this->_filePath);
	std::string response = "";
	Server::log("The file requested \"" + this->_filePath + "\" was found.", 1);
	std::string mimeType = getMimeType();

	response = firstHeader();
	response += "Content-Type: " + mimeType + "\r\n";
	std::ostringstream oss;
	oss << content.size();
	response += "Content-Length: " + oss.str() + "\r\n";
	response += "Connection: close\r\n";
	response += "Server: " + *this->_serverName.begin() + "\r\n\r\n";
	response += content;
	return (response);
}

/*response to a POST request*/
std::string Response::ft_post()
{
	Server::log("Server's receive a POST request.", 1);
	std::string content = readFileContent(this->_filePath);
	std::string response = "";

	std::string mimeType = getMimeType();

	response = firstHeader();
	response += "Content-Type: " + mimeType + "\r\n";
	std::ostringstream oss;
	oss << content.size();
	response += "Content-Length: " + oss.str() + "\r\n";
	response += "Connection: close\r\n";
	response += "Server: " + *this->_serverName.begin() + "\r\n\r\n";
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
		response += "Server: " + *this->_serverName.begin() + "\r\n\r\n";
		response += "File not found";

		return (response);
	}

	if (unlink(getFilePath().c_str()) == 0)
	{
		std::string response = "HTTP/1.1 204 No Content\r\n";
		response += "Connection: close\r\n";
		response += "Server: " + *this->_serverName.begin() + "\r\n\r\n";

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
	if (!this->_errorPages.empty() && (this->_errorPages.find(404) != this->_errorPages.end()))
	{
		content = readFileContent(this->_root + this->_errorPages.find(404)->second);
	} else {
		content = readFileContent("/home/mmahfoud/ecole_42/webserv/webserv_v1.0/www/error_pages/404.html");	
	}

	std::string response = "HTTP/1.1 400 Bad Request\r\n";
	response += "Content-Type: text/html\r\n";
	std::ostringstream oss;
    oss << content.size();
    response += "Content-Length: " + oss.str() + "\r\n";
	response += "Connection: close\r\n";
	response += "Server: " + *this->_serverName.begin() + "\r\n\r\n";
	response += content;
	
	return (response);
}

std::string Response::ft_forbidden()
{
	std::string content = readFileContent("/home/mmahfoud/ecole_42/webserv/webserv_v1.0/www/error_pages/403.html");

	std::string response = "HTTP/1.1 403 Bad Request\r\n";
	response += "Content-Type: text/html\r\n";
	std::ostringstream oss;
    oss << content.size();
    response += "Content-Length: " + oss.str() + "\r\n";
	response += "Connection: close\r\n";
	response += "Server: " + *this->_serverName.begin() + "\r\n\r\n";
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
	if (this->_filePath == this->_root + "/")
		return ("text/html");

	size_t ext = this->_filePath.rfind(".");
	if (ext != std::string::npos)
	{
		std::string extension = this->_filePath.substr(ext);
		std::cout << extension << std::endl;
		if (this->_mimePath.find(extension) != this->_mimePath.end())
		{
			return (this->_mimePath[extension]);
		}
	}
	Server::log("Extension of the files was not recognize.", 1);
	return ("application/octet-stream");
}

std::map<std::string, std::string>	Response::createMimePath()
{
	std::map<std::string, std::string> mimePath;

	mimePath[".sh"] = "text/html";
	mimePath[".py"] = "text/html";
	mimePath[".html"] = "text/html";
	mimePath[".css"] = "text/css";
	mimePath[".js"] = "application/javascript";
	mimePath[".json"] = "application/json";
	mimePath[".jpg"] = "image/jpeg";
	mimePath[".jpeg"] = "image/jpeg";
	mimePath[".png"] = "image/png";
	mimePath[".gif"] = "image/gif";
	mimePath[".bmp"] = "image/bmp";
	mimePath[".ico"] = "image/x-icon";
	mimePath[".webp"] = "image/webp";
	mimePath[".svg"] = "image/svg+xml";
	mimePath[".mp4"] = "video/mp4";
	mimePath[".webm"] = "video/webm";
	mimePath[".avi"] = "video/x-msvideo";
	mimePath[".mp3"] = "audio/mpeg";
	mimePath[".pdf"] = "application/pdf";
	mimePath[".xml"] = "application/xml";
	mimePath[".ttf"] = "font/ttf";
	mimePath[".woff"] = "font/woff";
	mimePath[".woff2"] = "font/woff2";
	mimePath[".csv"] = "text/csv";
	return (mimePath);
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
	// Cout << "server name	:" << response.getServerName() << std::endl;
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
