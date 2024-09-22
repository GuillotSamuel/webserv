/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 22:07:42 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/09/22 22:47:39 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "webserv.hpp"

class Response
{
	private:
		std::string							_serverName;
		std::string							_root;
		std::string							_index;
		int									_clientMaxBodySize;
		std::string							_cgiPath;
		std::map<std::string, std::string>	_pathInfoCgi;
		std::map<std::string, int>			_allowed_methods;
		std::map<int, std::string>			_errorPages;
		int									_autoIndex;
		std::string                 		_alias;
		std::string                 		_uploadsLocation;
		std::map<int, std::string>			_redirection;
		int									_status;

	public:
		Response();
		void	setInfo(ServerConfiguration *serv, Location *location);
		~Response();
};
