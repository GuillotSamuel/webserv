/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_configuration_methods.cpp                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:33:39 by sguillot          #+#    #+#             */
/*   Updated: 2024/07/10 11:23:32 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

ServerConfiguration::ServerConfiguration(void) :
    port(8080), hostName("127.0.0.1"),
    serverName("localHost"), errorPages(),
    clientMaxBodySize(10) {}

ServerConfiguration::ServerConfiguration(const ServerConfiguration &copy) :
    port(copy.port), hostName(copy.hostName),
    serverName(copy.serverName), errorPages(copy.errorPages),
    clientMaxBodySize(copy.clientMaxBodySize) {}

ServerConfiguration::~ServerConfiguration(void){}

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
    this->clientMaxBodySize = n;
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

std::string ServerConfiguration::getErrorPage(int code) const
{
    std::map<int, std::string>::const_iterator it = this->errorPages.find(code);
    if (it != this->errorPages.end()) {
        return it->second;
    }
    return ""; // TO MODIFY ALL FUNCTION
}

int	ServerConfiguration::getClientMaxBodySize(void) const
{
    return (this->clientMaxBodySize);
}
