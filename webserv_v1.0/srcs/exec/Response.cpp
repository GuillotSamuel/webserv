/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 22:12:59 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/09/23 10:52:40 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Response.hpp"

Response::Response()
{
	this->_serverName = "";
	this->_alias = "";
	this->_root = "";
	this->_clientMaxBodySize = 0;
	this->_autoIndex = -1;
	this->_index = "";
	this->_uploadsLocation = "";
}

void	Response::setInfo(ServerConfiguration *serv, Location *location)
{
	this->_serverName = serv->getServerName();
	if (location)
	{
		if (location->getRoot() != "")
			this->_root = location->getRoot();
		else
			this->_root = serv->getRoot();
		if (location->getIndex() != "")
			this->_root = location->getIndex();
		else
			this->_root = serv->getIndex();
		if (location->getClientMaxBodySize() != 0)
			this->_clientMaxBodySize = location->getClientMaxBodySize();
		else
			this->_clientMaxBodySize = serv->getClientMaxBodySize();
		if (location->getPathInfo() != "")
			this->_cgiPath = location->getPathInfo();
		else
			this->_cgiPath = serv->getCgiLocation();
		if (!location->getCgi().empty())
			this->_pathInfoCgi = location->getCgi();
		else
			this->_pathInfoCgi = serv->getPathInfoCgi();
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
		this->_alias = location->getAlias();
		if (!location->getUploadsLocation().empty())
			this->_uploadsLocation = location->getUploadsLocation();
		else
			this->_uploadsLocation = serv->getUploadLocation();
		if (!location->getRedirection().empty())
			this->_redirection = location->getRedirection();
		else
			this->_redirection = serv->getRedirection();
	}
	else {
		this->_root = serv->getRoot();
		this->_root = serv->getIndex();
		this->_clientMaxBodySize = serv->getClientMaxBodySize();
		this->_cgiPath = serv->getCgiLocation();
		this->_pathInfoCgi = serv->getPathInfoCgi();
		this->_allowed_methods = serv->getAllowedMethods();
		this->_errorPages = serv->getErrorPages();
		this->_autoIndex = serv->getAutoIndex();
		this->_uploadsLocation = serv->getUploadLocation();
		this->_redirection = serv->getRedirection();
	}
}

Response::~Response()
{
	this->_pathInfoCgi.clear();
	this->_allowed_methods.clear();
	this->_errorPages.clear();
	this->_redirection.clear();
}
