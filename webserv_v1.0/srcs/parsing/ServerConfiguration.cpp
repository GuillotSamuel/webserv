/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfiguration.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:33:39 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/12 12:03:22 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "ServerConfiguration.hpp"

// ServerConfiguration::ServerConfiguration(void) :
//     port(-1), hostName(std::string()),
//     serverName(std::string()), errorPages(std::map<int, std::string>()),
//     clientMaxBodySize(-1), _pathInfo(std::string()) {}

ServerConfiguration::ServerConfiguration()
{
	char *cRoot = getcwd(NULL, 0); // je suis ici ->  /home/mmahfoud/ecole_42/webserv/webserv_v1.0/
	std::string root_cpy(cRoot, strlen(cRoot));

	root_cpy += "/../website_gallery"; // a supprimer

	this->root = root_cpy;
	free(cRoot);
	// this->_index =  root + "/www/default.html";

	this->root_index = root + "/html/index.html"; // a supprimer
	this->_pathInfoCgi[".py"] = "/usr/bin/python3";
	this->errorPages[404] = root + "/www/error_pages/404.html";
	this->port = -1;
	this->_port.push_back(8080); // TEST BRUTFORCE
	this->_port.push_back(8081); // TEST BRUTFORCE
	this->_port.push_back(8082); // TEST BRUTFORCE

	this->hostName = std::string("");
	this->serverName = std::string("localhost"); // TEST PARSING
}

void ServerConfiguration::creatMultiPort()
{
	std::vector<int>::iterator it = this->_port.begin();
	for (; it < this->_port.end(); it++)
	{
		this->tab_list.push_back(new ListeningSocket(*it));
	}
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

void ServerConfiguration::setPort(std::string str)
{
	int n;
	std::istringstream(str) >> n;
	this->_port.push_back(n);
}

void ServerConfiguration::setLocation(std::string page, std::string location)
{
	this->_location.insert(std::make_pair(page, location));
}

void ServerConfiguration::setHostName(std::string str)
{
	this->hostName = str;
}

void ServerConfiguration::setServerName(std::string str)
{
	this->serverName = str;
}

void ServerConfiguration::setErrorPage(int code, std::string str)
{
	this->errorPages[code] = str;
}

void ServerConfiguration::setClientMaxBodySize(std::string str)
{
	int n;
	std::istringstream(str) >> n;
	this->clientMaxBodySize = n;
}

void ServerConfiguration::setRoot(std::string str)
{
	this->root = str;
}

void ServerConfiguration::setIndex(std::string str)
{
	this->index = str;
}

int ServerConfiguration::getPort(void) const
{
	return (this->port);
}

std::vector<int> ServerConfiguration::getPortTab(void) const
{
	return (this->_port);
}

std::string ServerConfiguration::getHostName(void) const
{
	return (this->hostName);
}

std::string ServerConfiguration::getServerName(void) const
{
	return (this->serverName);
}

std::string ServerConfiguration::getStrPort(void) const
{
	return (this->strPort);
}

std::string ServerConfiguration::getRoot(void) const
{
	return (this->root);
}

std::string ServerConfiguration::getRootIndex(void) const
{
	return (this->root_index);
}

std::string ServerConfiguration::getErrorPage(int code) const
{
	std::map<int, std::string>::const_iterator it = this->errorPages.find(code);
	if (it != this->errorPages.end())
	{
		return it->second;
	}
	else
	{
		it = this->errorPages.find(404);
		return (it->second);
	}
}

int ServerConfiguration::getClientMaxBodySize(void) const
{
	return (this->clientMaxBodySize);
}

std::map<std::string, std::string> ServerConfiguration::getPathInfoCgi() const
{
	return (this->_pathInfoCgi);
}

std::vector<ListeningSocket *> ServerConfiguration::getTabList() const
{
	return (this->tab_list);
}

std::ostream &operator<<(std::ostream &Cout, ServerConfiguration const &i)
{
	Cout << i.getClientMaxBodySize() << std::endl;
	Cout << i.getHostName() << std::endl;
	Cout << i.getPort() << std::endl;
	Cout << i.getServerName() << std::endl;
	return (Cout);
}

void ServerConfiguration::error(std::string errorType)
{
	throw(std::runtime_error(errorType));
}