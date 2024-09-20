/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Creted: 2024/09/20 11:51:44 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/09/20 14:17:32 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "webserv.hpp"

class Location
{
	private:
		std::string							_blockName;
		std::string							_blockType; // equal / prefixe / etc
		std::string                 		_alias; // remplace l'URI par cette variable
		std::string                 		_root; // permet de trouver le path qui mene a ce fichier
		int                 				_clientMaxBodySize;
		int                         		_autoindex;
		std::string                 		_path_info;
		std::string                 		_index;
		std::string                 		_uploadsLocation;
		std::map<std::string, std::string>	_cgi;
		std::map<int, std::string>			_error_page;
		std::map<int, std::string>			_redirection;
		std::map<std::string, int>  		_allowed_methods;
	public:
		Location(/* args */);
		~Location();
};

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
	this->_allowed_methods["GET"] = 0;
	this->_allowed_methods["POST"] = 0;
}

Location::~Location()
{
	this->_cgi.clear();
	this->_error_page.clear();
	this->_redirection.clear();
	this->_allowed_methods.clear();
}
