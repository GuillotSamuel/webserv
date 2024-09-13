/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfiguration.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:33:39 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/13 12:41:04 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "ServerConfiguration.hpp"

ServerConfiguration::ServerConfiguration()
{
	char *tmp = getcwd(NULL, 0);
	std::string tmp2(tmp, strlen(tmp));
	free(tmp);
	this->hostName = "";
	this->serverName = "";
	this->strPort = "";
	this->imHere = tmp2;
	this->root = "";
	this->root_index = "";
	this->index = "";
	this->uploadsLocation = "";
	this->errorPagesLocation = "";
	this->cgiBin_location = "";
}

ServerConfiguration::~ServerConfiguration(void)
{
	this->_pathInfoCgi.clear();
	this->_pathInfoMime.clear();
	this->errorPages.clear();
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

void ServerConfiguration::setRootIndex()
{
	this->root_index = this->imHere + this->root + HTML_FILES + "/" + this->index;
}

void ServerConfiguration::setLocation(std::string page, std::string location)
{
	this->_location.insert(std::make_pair(page, location));
}

void ServerConfiguration::setAllowedMethods(std::vector<std::string> allowed_methods)
{
	this->allowed_methods = allowed_methods;
}

void ServerConfiguration::setPort(std::string str)
{
	int n;
	std::istringstream(str) >> n;
	this->_port.push_back(n);
}

void ServerConfiguration::setUploadsLocation(std::string str)
{
	this->uploadsLocation = str;
}

void ServerConfiguration::setErrorPagesLocation(std::string str)
{
	this->errorPagesLocation = str;
}

void ServerConfiguration::setPathInfoCgi(std::string extension, std::string location)
{
	this->_location.insert(std::make_pair(extension, location));
}

void ServerConfiguration::setCgiBinLocation(std::string str)
{
	this->cgiBin_location = str;
}

void ServerConfiguration::setHostName(std::string str)
{
	this->hostName = str;
}

void ServerConfiguration::setServerName(std::string str)
{
	this->serverName = str;
}

void ServerConfiguration::setErrorPages(int code, std::string str)
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

std::string ServerConfiguration::getUploadLocation() const
{
	return (this->uploadsLocation);
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

std::string ServerConfiguration::getCgiLocation(void) const
{
	return (this->cgiBin_location);
}


std::string ServerConfiguration::getErrorPageLocation(void) const
{
	return (this->errorPagesLocation);
}

std::map<std::string, std::string> ServerConfiguration::getLocation(void) const
{
	return (this->_location);
}

std::vector<std::string> ServerConfiguration::getAllowedMethods(void) const
{
	return (this->allowed_methods);
}

std::string ServerConfiguration::getimHere() const
{
	return (this->imHere);
}

std::map<std::string, std::string> ServerConfiguration::getInfoMime(void) const
{
	return (this->_pathInfoMime);
}

std::map<int, std::string> ServerConfiguration::getErrorPages(void) const
{
	return (this->errorPages);
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

std::string ServerConfiguration::getIndex() const
{
	return (this->index);
}

std::ostream &operator<<(std::ostream &Cout, ServerConfiguration const &sc)
{
	Cout << "Server name : " << sc.getServerName() << "\n\n";
	Cout << "Path info mime map : \n";
	std::map<std::string, std::string> info_mime_tab = sc.getInfoMime();
	std::map<std::string, std::string>::iterator info_mime_it = info_mime_tab.begin();
	for (; info_mime_it != info_mime_tab.end(); info_mime_it++)
		Cout << info_mime_it->first << " : " << info_mime_it->second << "\n";
	Cout << "Path info cgi map : \n";
	std::map<std::string, std::string> path_info_tab = sc.getPathInfoCgi();
	std::map<std::string, std::string>::iterator path_info_it = path_info_tab.begin();
	for (; path_info_it != path_info_tab.end(); path_info_it++)
		Cout << path_info_it->first << " : " << path_info_it->second << "\n";
	Cout << "Location map : \n";
	std::map<std::string, std::string> location_tab = sc.getLocation();
	std::map<std::string, std::string>::iterator location_it = location_tab.begin();
	for (; location_it != location_tab.end(); location_it++)
		Cout << location_it->first << " : " << location_it->second << "\n";
	Cout << "Error pages map: \n";
	std::map<int, std::string> error_pages_tab = sc.getErrorPages();
	std::map<int, std::string>::iterator error_pages_it = error_pages_tab.begin();
	for (; error_pages_it != error_pages_tab.end(); error_pages_it++)
		Cout << error_pages_it->first << " : " << error_pages_it->second << "\n";
	Cout << "Host name : " << sc.getHostName() << "\n";
	Cout << "Server name : " << sc.getServerName() << "\n";
	Cout << "Str Port : " << sc.getStrPort() << "\n";
	Cout << "Root : " << sc.getRoot() << "\n";
	Cout << "Root index : " << sc.getRootIndex() << "\n";
	Cout << "Index : " << sc.getIndex() << "\n";
	Cout << "Upload location : " << sc.getUploadLocation() << "\n";
	Cout << "Error Page Location : " << sc.getErrorPageLocation() << "\n";
	Cout << "Cgi-bin location : " << sc.getCgiLocation() << "\n";
	Cout << "Port vector : \n";
	std::vector<int> port_tab = sc.getPortTab();
	std::vector<int>::iterator port_it = port_tab.begin();
	for (; port_it < port_tab.end(); port_it++)
		Cout << *port_it << "\n";
	Cout << "Allowed methods vector : \n";
	std::vector<std::string> allowed_methods_tab = sc.getAllowedMethods();
	std::vector<std::string>::iterator allowed_methods_it = allowed_methods_tab.begin();
	for (; allowed_methods_it < allowed_methods_tab.end(); allowed_methods_it++)
		Cout << *allowed_methods_it << "\n";
	Cout << "Port : " << sc.getPort() << "\n";
	Cout << "Client max body size : " << sc.getClientMaxBodySize() << "\n";	

	return (Cout);
}

void ServerConfiguration::error(std::string errorType)
{
	throw(std::runtime_error(errorType));
}