/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 12:40:40 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/09/23 17:44:26 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Location.hpp"

Location::Location(/* args */)
{
	this->_blockName = "";
	this->_blockType = "";
	this->_alias = "";
	this->_root = "";
	this->_clientMaxBodySize = 0;
	this->_autoindex = 0;
	this->_path_info = "";
	this->_index = "";
	this->_uploadsLocation = "";
	this->_allowed_methods["GET"] = -1;
	this->_allowed_methods["POST"] = -1;
	this->allowd_methods["DELETE"] = -1;
}

Location::~Location()
{
	this->_cgi.clear();
	this->_error_page.clear();
	this->_redirection.clear();
	this->_allowed_methods.clear();
}

void Location::setBlockName(std::string blockName)
{
	this->_blockName = blockName;
}

void Location::setBlockType(std::string blockType)
{
	this->_blockType = blockType;
}

void Location::setAlias(std::string alias)
{
	this->_alias = alias;
}

void Location::setRoot(std::string root)
{
	this->_root = root;
}

void Location::setClientMaxBodySize(int clientMaxBodySize)
{
	this->_clientMaxBodySize = clientMaxBodySize;
}

void Location::setAutoIndex(int autoIndex)
{
	this->_autoindex = autoIndex;
}

void Location::setIndex(std::string index)
{
	this->_index = index;
}

void Location::setUploadsLocation(std::string uploadsLocation)
{
	this->_uploadsLocation = uploadsLocation;
}

void Location::setCgi(std::string first, std::string second)
{
	this->_cgi[first] = second;
}

void Location::setErrorPage(int code, std::string errorPage)
{
	this->_error_page[code] = errorPage;
}

void Location::setRedirection(int code, std::string to)
{
	this->_redirection[code] = to;
}

void Location::setAllowedMethods(std::string whichOne, int allow)
{
	(void)whichOne; // TEST
	(void)allow; // TEST
}

std::string Location::getBlockName() const
{
	return (this->_blockName);
}

std::string Location::getBlockType() const
{
	return (this->_blockType);
}

std::string Location::getAlias() const
{
	return (this->_alias);
}

std::string Location::getRoot() const
{
	return (this->_root);
}

int Location::getClientMaxBodySize() const
{
	return (this->_clientMaxBodySize);
}

int Location::getAutoIndex() const
{
	return (this->_autoindex);
}

std::string Location::getPathInfo() const
{
	return (this->_path_info);
}

std::string Location::getIndex() const
{
	return (this->_index);
}

std::string Location::getUploadsLocation() const
{
	return (this->_uploadsLocation);
}

std::map<std::string, std::string> Location::getCgi() const
{
	return (this->_cgi);
}

std::map<int, std::string> Location::getErrorPage() const
{
	return (this->_error_page);
}

std::map<int, std::string> Location::getRedirection() const
{
	return (this->_redirection);
}

int Location::getAllowedMethods(std::string wichOne)
{
}
