/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfiguration.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:33:39 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/01 19:52:19 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

// ServerConfiguration::ServerConfiguration(void) :
//     port(-1), hostName(std::string()),
//     serverName(std::string()), errorPages(std::map<int, std::string>()),
//     clientMaxBodySize(-1), _pathInfo(std::string()) {}

ServerConfiguration::ServerConfiguration(int argc, char **argv)
{
	this->_log = new std::ofstream("log.txt");

	this->port = 8090;
	this->hostName = std::string("127.0.0.1");
	this->serverName = std::string("localhost");
	this->errorPages[404] = std::string("404.html");
	this->_pathInfoCgi[(".py")] = std::string("/usr/bin/python3");
	this->_pathInfoCgi[(".sh")] = std::string("/bin/bash");
	this->_pathInfoMime[("HTML_FILES")] = std::string("www/html");
	this->_pathInfoMime[("CSS_FILES")] = std::string("www/styles");
	this->_pathInfoMime[("JS_FILES")] = std::string("www/js");
	this->_pathInfoMime[("JSON_FILES")] = std::string("www/data");
	this->_pathInfoMime[("IMAGE_FILES")] = std::string("www/images");
	this->_pathInfoMime[("VIDEO_FILES")] = std::string("www/videos");
	this->_pathInfoMime[("AUDIO_FILES")] = std::string("www/audio");
	this->_pathInfoMime[("FONT_FILES")] = std::string("www/fonts");
	this->_pathInfoMime[("PDF_FILES")] = std::string("www/docs");
	this->_pathInfoMime[("XML_FILES")] = std::string("www/xml");
	this->_pathInfoMime[("ICON_FILES")] = std::string("www/icons");
	this->_pathInfoMime[("CSV_FILES")] = std::string("www/data");
	this->_pathInfoMime[("ERROR_400_PAGE")] = std::string("www/html/errors/400.html");
	
	parsing_g(argc, argv);
	
	log("configuration of the server finished.", 3);
}

ServerConfiguration::ServerConfiguration(const ServerConfiguration &copy) : port(copy.port), hostName(copy.hostName),
																			serverName(copy.serverName), errorPages(copy.errorPages),
																			clientMaxBodySize(copy.clientMaxBodySize) {}

ServerConfiguration::~ServerConfiguration(void)
{
	this->_pathInfoCgi.clear();
	this->_pathInfoMime.clear();
	this->errorPages.clear();
	this->_log->close();
	delete this->_log;
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
	this->port = n;
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

int ServerConfiguration::getPort(void) const
{
	return (this->port);
}

std::string ServerConfiguration::getHostName(void) const
{
	return (this->hostName);
}

std::string ServerConfiguration::getServerName(void) const
{
	return (this->serverName);
}

std::string ServerConfiguration::getErrorPage(int code) const
{
	std::map<int, std::string>::const_iterator it = this->errorPages.find(code);
	if (it != this->errorPages.end())
	{
		return it->second;
	}
	return (""); // TO MODIFY ALL FUNCTION TEST
}

int ServerConfiguration::getClientMaxBodySize(void) const
{
	return (this->clientMaxBodySize);
}

std::map<std::string, std::string> ServerConfiguration::getPathInfoCgi() const
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

void ServerConfiguration::log(std::string error, int type)
{
	if (this->_log && this->_log->is_open())
	{
		if (type == 1)
			*this->_log << "\t[INFO] :" << error << std::endl;
		else if (type == 2)
			*this->_log << "\t[ERROR] : " << error << std::endl;
		else if (type == 3)
			*this->_log << "[INFO] : " << error << std::endl;
	}
	else
	{
		std::cerr << "Log error: Log stream is null or not open" << std::endl;
	}
}

void	ServerConfiguration::error(std::string errorType)
{
	throw(std::runtime_error(errorType));
}