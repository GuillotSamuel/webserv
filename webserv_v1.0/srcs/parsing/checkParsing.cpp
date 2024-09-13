/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkParsing.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 15:45:09 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/13 18:31:15 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

#include <sys/stat.h>
#include <errno.h>

void Server::check_error_page(ServerConfiguration server_conf)
{
	(void)server_conf;
}

void Server::check_host_page(ServerConfiguration server_conf)
{
	(void)server_conf;
}

void Server::check_index(ServerConfiguration server_conf)
{
	(void)server_conf;
}

void Server::check_listen(ServerConfiguration server_conf)
{
	(void)server_conf;
}

void Server::check_location(ServerConfiguration server_conf)
{
	(void)server_conf;
/*	std::string path = server_conf.getRoot() + server_conf.getLocation();
	
 	struct stat info;

	if (stat(server_conf.getRoot().c_str(), &info) != 0)
	{
		error("Error: Cannot access root directory: " + std::string(strerror(errno)) + " (" + server_conf.getRoot() + ")" +
			  " / server name -> " +
			  server_conf.getServerName());
	}
	else if (info.st_mode & S_IFDIR)
	{
		return;
	}
	else
	{
		error("Error: The root path is not a directory / server name -> " + server_conf.getServerName());
	} */
}

void Server::check_max_body(ServerConfiguration server_conf)
{
	(void)server_conf;
}

void Server::check_root(ServerConfiguration server_conf)
{
	struct stat info;

	if (stat(server_conf.getRoot().c_str(), &info) != 0)
	{
		error("Error: Cannot access root directory: " + std::string(strerror(errno)) + " (" + server_conf.getRoot() + ")" +
			  " / server name -> " +
			  server_conf.getServerName());
	}
	else if (info.st_mode & S_IFDIR)
	{
		return;
	}
	else
	{
		error("Error: The root path is not a directory / server name -> " + server_conf.getServerName());
	}
}

void Server::check_server_name(ServerConfiguration server_conf)
{
	(void)server_conf;
}

void Server::check_parsing()
{
	std::vector<ServerConfiguration> current_tab_serv = this->tab_serv;
	std::vector<ServerConfiguration>::iterator iterator_tab_serv = current_tab_serv.begin();
	for (; iterator_tab_serv != current_tab_serv.end(); iterator_tab_serv++)
	{
		check_root(*iterator_tab_serv);
		check_error_page(*iterator_tab_serv);
		check_host_page(*iterator_tab_serv);
		check_index(*iterator_tab_serv);
		check_listen(*iterator_tab_serv);
		check_location(*iterator_tab_serv);
		check_max_body(*iterator_tab_serv);
		check_server_name(*iterator_tab_serv);
	}
}
