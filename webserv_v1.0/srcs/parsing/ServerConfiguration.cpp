/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfiguration.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <mmahfoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:33:39 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/10 15:46:54 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "ServerConfiguration.hpp"

// ServerConfiguration::ServerConfiguration(void) :
//     port(-1), hostName(std::string()),
//     serverName(std::string()), errorPages(std::map<int, std::string>()),
//     clientMaxBodySize(-1), _pathInfo(std::string()) {}

ServerConfiguration::ServerConfiguration() {
    
    char *cRoot = getcwd(NULL, 0);// je suis ici ->  /home/mmahfoud/ecole_42/webserv/webserv_v1.0
    std::string root_cpy(cRoot, strlen(cRoot));
    this->root = root_cpy;
    free(cRoot);
    std::string def_index =  root + "/www/default.html";
    this->errorPages[404] = root + "/www/error_pages/404.html";
    this->port = -1;
    this->hostName = std::string("");
    this->serverName = std::string("localhost"); // TEST PARSING
}



// ServerConfiguration::ServerConfiguration(const ServerConfiguration &copy) :
//     port(copy.port), hostName(copy.hostName),
//     serverName(copy.serverName), errorPages(copy.errorPages),
//     clientMaxBodySize(copy.clientMaxBodySize) {}

ServerConfiguration::~ServerConfiguration(void)
{
    this->_pathInfoCgi.clear();
    this->_pathInfoMime.clear();
    this->errorPages.clear();
    // this->_log->close();
    // delete this->_log;
}

ServerConfiguration &ServerConfiguration::operator=(const ServerConfiguration &copy)
{
    if (this != &copy)
    {
        this->port = copy.port;
        this->hostName = copy.hostName;
        this->serverName = copy.serverName;
        this->errorPages = copy.errorPages;
        this->clientMaxBodySize = copy.clientMaxBodySize;
    }
    return (*this);
}

void	ServerConfiguration::setPort(std::string str)
{
    int n;
    std::istringstream(str) >> n;
    this->port = n;
}

void	ServerConfiguration::setHostName(std::string str)
{
    this->hostName = str;
}

void	ServerConfiguration::setServerName(std::string str)
{
    this->serverName = str;
}

void	ServerConfiguration::setErrorPage(int code, std::string str)
{
    this->errorPages[code] = str;
}

void	ServerConfiguration::setClientMaxBodySize(std::string str)
{
    int n;
    std::istringstream(str) >> n;
    this->clientMaxBodySize = n;
}

int		ServerConfiguration::getPort(void) const
{
    return (this->port);
}

std::string	ServerConfiguration::getHostName(void) const
{
    return (this->hostName);   
}

std::string	ServerConfiguration::getServerName(void) const
{
    return (this->serverName);
}

std::string	ServerConfiguration::getStrPort(void) const
{
    return (this->strPort);
}

std::string ServerConfiguration::getRoot(void) const
{
    return (this->root);
}

std::string ServerConfiguration::getErrorPage(int code) const
{
    std::map<int, std::string>::const_iterator it = this->errorPages.find(code);
    if (it != this->errorPages.end()) {
        return it->second;
    }
    else
    {
        it = this->errorPages.find(404);
        return (it->second);
    }
}

int	ServerConfiguration::getClientMaxBodySize(void) const
{
    return (this->clientMaxBodySize);
}

std::map<std::string, std::string>	ServerConfiguration::getPathInfoCgi() const
{
    return (this->_pathInfoCgi);
}

std::ostream &operator<<(std::ostream &Cout, ServerConfiguration const &i)
{
	Cout << i.getClientMaxBodySize() << std::endl;
    Cout << i.getHostName() << std::endl;
    Cout << i.getPort() << std::endl;
    Cout << i.getServerName() << std::endl;
    return (Cout);
}

void	ServerConfiguration::error(std::string errorType)
{
	throw(std::runtime_error(errorType));
}