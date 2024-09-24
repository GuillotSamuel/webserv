/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfiguration.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:33:39 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/24 21:55:21 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfiguration.hpp"
#include "webserv.hpp"

ServerConfiguration::ServerConfiguration()
{
	char *tmp = getcwd(NULL, 0);
	std::string tmp2(tmp, strlen(tmp));
	free(tmp);
	this->strPort = "";
	this->imHere = tmp2 + "/";
	this->_root = "";
	this->root_index = "";
	this->_index = "";
	this->_uploadsLocation = "";
	this->_errorPagesLocation = "";
	this->_errorPages[400] = "400.html";
	this->_errorPages[404] = "404.html";
	this->_cgiBin_location = "";
	this->_allowed_methods["GET"] = -1;
	this->_allowed_methods["POST"] = -1;
	this->_allowed_methods["DELETE"] = -1;
}

ServerConfiguration::~ServerConfiguration(void)
{
	this->_pathInfoCgi.clear();
	this->_pathInfoMime.clear();
	this->_errorPages.clear();
	this->_interpreter_map.clear();
	this->_allowed_methods.clear();
	this->_locations_map.clear();
	// this->_location.clear();
}

void ServerConfiguration::setRootIndex()
{
	this->root_index = this->_root + this->_index;
}

void ServerConfiguration::setInterpreterMap(std::string page, std::string interpreter)
{
	this->_interpreter_map.insert(std::make_pair(page, interpreter));
}

void ServerConfiguration::setAllowedMethods(std::string method, int code)
{
	this->_allowed_methods[method] = code;
}

void ServerConfiguration::setUploadsLocation(std::string str)
{
	this->_uploadsLocation = str;
}

void ServerConfiguration::setErrorPagesLocation(std::string str)
{
	this->_errorPagesLocation = str;
}

void ServerConfiguration::setPathInfoCgi(std::string extension, std::string interpreter)
{
	this->_interpreter_map.insert(std::make_pair(extension, interpreter));
}

void ServerConfiguration::setCgiBinLocation(std::string str)
{
	this->_cgiBin_location = str;
}

void ServerConfiguration::setListen(std::string address, std::string port)
{
	this->_portList.insert(std::make_pair(address, port));
}

void ServerConfiguration::setServerName(std::string str)
{
	this->_serverName.push_back(str);
}

void ServerConfiguration::setErrorPages(int code, std::string str)
{
	this->_errorPages[code] = str;
}

void ServerConfiguration::setClientMaxBodySize(std::string str)
{
	int n;
	std::istringstream(str) >> n;
	this->_clientMaxBodySize = n;
}

void ServerConfiguration::setRoot(std::string str)
{
	this->_root = str;
}

void ServerConfiguration::setAutoIndex(std::string str)
{
	if (str == "on")
	{
		this->_autoIndex = 1;
	}
	else if (str == "off")
	{
		this->_autoIndex = 0;
	}
}

void ServerConfiguration::setIndex(std::string str)
{
	this->_index = str;
}

void ServerConfiguration::setBlockLocation(Location new_location)
{
	this->_location.push_back(new_location);
}

std::multimap<std::string, std::string> ServerConfiguration::getPortList(void) const
{
	return (this->_portList);
}

std::vector<std::string> ServerConfiguration::getServerName(void) const
{
	return (this->_serverName);
}

std::string ServerConfiguration::getStrPort(void) const
{
	return (this->strPort);
}

std::string ServerConfiguration::getRoot(void) const
{
	return (this->_root);
}

std::string ServerConfiguration::getRootIndex(void) const
{
	return (this->root_index);
}

std::string ServerConfiguration::getUploadLocation() const
{
	return (this->_uploadsLocation);
}

std::string ServerConfiguration::getAutoIndexStr() const
{
	if (this->_autoIndex == 1)
		return ("on");
	else
		return ("off");
}

int ServerConfiguration::getAutoIndex() const
{
	return (this->_autoIndex);
}

std::string ServerConfiguration::getErrorPage(int code) const
{
	std::map<int, std::string>::const_iterator it = this->_errorPages.find(code);
	if (it != this->_errorPages.end())
	{
		return it->second;
	}
	else
	{
		it = this->_errorPages.find(404);
		return (it->second);
	}
}

std::string ServerConfiguration::getCgiLocation(void) const
{
	return (this->_cgiBin_location);
}

std::string ServerConfiguration::getErrorPageLocation(void) const
{
	return (this->_errorPagesLocation);
}

std::map<std::string, std::string> ServerConfiguration::getInterpreterMap(void) const
{
	return (this->_interpreter_map);
}

std::map<std::string, int> ServerConfiguration::getAllowedMethods(void) const
{
	return (this->_allowed_methods);
}

std::string ServerConfiguration::getimHere() const
{
	return (this->imHere);
}

std::map<std::string, std::string> ServerConfiguration::getInfoMime(void) const
{
	return (this->_pathInfoMime);
}

std::map<std::string, t_location> ServerConfiguration::getTabLocation(void) const
{
	return (this->_locations_map);
}

std::map<int, std::string> ServerConfiguration::getErrorPages(void) const
{
	return (this->_errorPages);
}

int ServerConfiguration::getClientMaxBodySize(void) const
{
	return (this->_clientMaxBodySize);
}

std::map<std::string, std::string> ServerConfiguration::getPathInfoCgi() const
{
	return (this->_pathInfoCgi);
}

std::string ServerConfiguration::getIndex() const
{
	return (this->_index);
}

std::vector<Location> ServerConfiguration::getLocation() const
{
	return (this->_location);
}

std::ostream &operator<<(std::ostream &Cout, ServerConfiguration const &sc)
{
	Cout << YELLOW << "\n---------------------------------------------------------------------\n\n"
		 << RESET;

	Cout << WHITE << "Server name : \n";
	std::vector<std::string> server_name_tab = sc.getServerName();
	std::vector<std::string>::iterator server_name_it = server_name_tab.begin();
	for (; server_name_it != server_name_tab.end(); server_name_it++)
		Cout << CYAN << *server_name_it << RESET << "\n";
	Cout << "\n";

	// Cout << WHITE << "Host name : " << RESET << CYAN << sc.getHostName() << RESET << "\n\n";

	Cout << WHITE << "Port vector : \n"
		 << RESET;
	std::multimap<std::string, std::string> port_tab = sc.getPortList();
	std::multimap<std::string, std::string>::iterator port_it = port_tab.begin();
	for (; port_it != port_tab.end(); port_it++)
		Cout << CYAN << port_it->first << ":" << port_it->second << RESET << "\n";
	Cout << "\n";

	Cout << WHITE << "Root : " << RESET << CYAN << sc.getRoot() << RESET << "\n\n";

	Cout << WHITE << "Index : " << RESET << CYAN << sc.getIndex() << RESET << "\n\n";

	Cout << WHITE << "Client max body size : " << RESET << CYAN << sc.getClientMaxBodySize() << RESET << "\n\n";

	Cout << WHITE << "Path info cgi map : \n"
		 << RESET;
	std::map<std::string, std::string> path_info_tab = sc.getPathInfoCgi();
	std::map<std::string, std::string>::iterator path_info_it = path_info_tab.begin();
	for (; path_info_it != path_info_tab.end(); path_info_it++)
		Cout << CYAN << path_info_it->first << " : " << path_info_it->second << RESET << "\n";
	Cout << "\n";

	// Cout << WHITE << "Path info mime map : \n" << RESET;
	// std::map<std::string, std::string> info_mime_tab = sc.getInfoMime();
	// std::map<std::string, std::string>::iterator info_mime_it = info_mime_tab.begin();
	// for (; info_mime_it != info_mime_tab.end(); info_mime_it++)
	// 	Cout << CYAN << info_mime_it->first << " : " << info_mime_it->second << RESET << "\n";
	// Cout << "\n";

	Cout << WHITE << "Allowed methods map : \n"
		 << RESET;
	std::map<std::string, int> allowed_methods_tab = sc.getAllowedMethods();
	std::map<std::string, int>::iterator allowed_methods_it = allowed_methods_tab.begin();
	for (; allowed_methods_it != allowed_methods_tab.end(); allowed_methods_it++)
		Cout << CYAN << allowed_methods_it->first << " : " << allowed_methods_it->second << RESET << "\n";
	Cout << "\n";

	Cout << WHITE << "Error pages map: \n"
		 << RESET;
	std::map<int, std::string> error_pages_tab = sc.getErrorPages();
	std::map<int, std::string>::iterator error_pages_it = error_pages_tab.begin();
	for (; error_pages_it != error_pages_tab.end(); error_pages_it++)
		Cout << CYAN << error_pages_it->first << " : " << error_pages_it->second << RESET << "\n";
	Cout << "\n";

	Cout << WHITE << "Auto Index : " << RESET << CYAN << sc.getAutoIndex() << RESET << "\n\n";

	Cout << WHITE << "Error Page Location : " << RESET << CYAN << sc.getErrorPageLocation() << RESET << "\n\n";

	Cout << WHITE << "Cgi-bin interpreter : " << RESET << CYAN << sc.getCgiLocation() << RESET << "\n\n";

	Cout << WHITE << "Upload interpreter : " << RESET << CYAN << sc.getUploadLocation() << RESET << "\n\n";

	Cout << WHITE << "Interpreters path map : \n"
		 << RESET;
	std::map<std::string, std::string> interpreter_tab = sc.getInterpreterMap();
	std::map<std::string, std::string>::iterator interpreter_it = interpreter_tab.begin();
	for (; interpreter_it != interpreter_tab.end(); interpreter_it++)
		Cout << CYAN << interpreter_it->first << " : " << interpreter_it->second << RESET << "\n";
	Cout << "\n";

	Cout << WHITE << "Location map : \n"
		 << RESET;
	std::vector<Location> location_tab = sc.getLocation();
	std::vector<Location>::iterator location_it = location_tab.begin();
	for (; location_it != location_tab.end(); location_it++)
	{
		Cout << GREEN << "*****************************" << RESET << "\n";
		Cout << "Block name : " << GREEN << BOLD << location_it->getBlockName() << RESET << "\n\n";
		Cout << "Alias : " << GREEN << location_it->getAlias() << RESET << "\n";
		Cout << "Root : " << GREEN << location_it->getRoot() << RESET << "\n";
		Cout << "Client Max Body Size : " << GREEN << location_it->getClientMaxBodySize() << RESET << "\n";
		Cout << "Auto index : " << GREEN << location_it->getAutoIndex() << RESET << "\n";
		// Cout << "Path Info : " << GREEN << location_it->getPathInfo() << RESET << "\n";
		Cout << "Index : " << GREEN << location_it->getIndex() << RESET << "\n";
		Cout << "Uploads location : " << GREEN << location_it->getUploadsLocation() << RESET << "\n";
		Cout << WHITE << "Allowed methods map : \n"
			 << RESET;
		std::map<std::string, int> location_allowed_methods_tab = location_it->getAllowedMethodsTab();
		std::map<std::string, int>::iterator location_allowed_methods_it = location_allowed_methods_tab.begin();
		for (; location_allowed_methods_it != location_allowed_methods_tab.end(); location_allowed_methods_it++)
			Cout << GREEN << location_allowed_methods_it->first << " : " << location_allowed_methods_it->second << RESET << "\n";
		Cout << "\n";
		
		Cout << WHITE << "Error pages : \n"
			 << RESET;
		std::map<int, std::string> error_tab = location_it->getErrorPage();
		std::map<int, std::string>::iterator error_it = error_tab.begin();
		for (; error_it != error_tab.end(); error_it++)
			Cout << GREEN << error_it->first << " : " << error_it->second << RESET << "\n";
		Cout << "\n";
		
		Cout << WHITE << "Redirections : \n";
		std::map<int, std::string> redirection_tab = location_it->getRedirection();
		std::map<int, std::string>::iterator redirection_it = redirection_tab.begin();
		for (; redirection_it != redirection_tab.end(); redirection_it++)
			Cout << GREEN << redirection_it->first << " : " << redirection_it->second << RESET << "\n";
		Cout << "\n";
		
		Cout << "Cgi path : " << GREEN << location_it->getPathCgi() << RESET << "\n\n";
		
		Cout << WHITE << "Cgi : \n"
			 << RESET;
		std::map<std::string, std::string> cgi_tab = location_it->getCgi();
		std::map<std::string, std::string>::iterator cgi_it = cgi_tab.begin();
		for (; cgi_it != cgi_tab.end(); cgi_it++)
			Cout << GREEN << cgi_it->first << " : " << cgi_it->second << RESET << "\n";
		Cout << "\n";

		Cout << GREEN << "*****************************" << RESET << "\n";
	}
	Cout << "\n";

	Cout << YELLOW << "---------------------------------------------------------------------\n"
		 << RESET;

	return (Cout);
}

void ServerConfiguration::error(std::string errorType)
{
	throw(std::runtime_error(errorType));
}
