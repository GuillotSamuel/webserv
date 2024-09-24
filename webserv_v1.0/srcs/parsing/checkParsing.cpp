/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkParsing.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 15:45:09 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/24 22:57:09 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

#include <sys/stat.h>
#include <errno.h>

void Server::check_error_page(ServerConfiguration server_conf)
{
	// check : if no lcation , what to do with pages / opo situation also
	(void)server_conf;
}

void Server::check_host_page(ServerConfiguration server_conf)
{
	(void)server_conf;
	// check: possible de mettre plusieurs hostnames en commun sur plusieurs servers + plusieurs hostnames par server ?
}

void Server::check_index(ServerConfiguration server_conf)
{
	std::string root_path = server_conf.getRoot();
	std::string index = server_conf.getIndex();
	std::string index_path = root_path + "/" + index;

	std::ifstream file(index_path.c_str());
	if (!file.good())
	{
		error("Error : Index file does not exist: " + index_path);
	}
}

void Server::check_listen(ServerConfiguration server_conf)
{
	(void)server_conf;
}

void Server::check_location(ServerConfiguration server_conf)
{
	(void)server_conf;
}

void Server::check_max_body(ServerConfiguration server_conf)
{
	long maxBodySize = server_conf.getClientMaxBodySize();

	if (maxBodySize <= 0)
	{
		error("Error: Invalid max body size. Must be a positive value.");
		return;
	}
}

void Server::check_root(ServerConfiguration server_conf)
{
	struct stat info;

	if (!server_conf.getRoot().empty())
	{
		if (stat(server_conf.getRoot().c_str(), &info) != 0)
		{
			std::string serverName = !server_conf.getServerName().empty() ? server_conf.getServerName()[0] : "Unknown Server";
			error("Error: Cannot access root directory: " + std::string(strerror(errno)) +
				  " (" + server_conf.getRoot() + ") / server name -> " + serverName);
		}
		else if (S_ISDIR(info.st_mode))
		{
			if (!(info.st_mode & S_IRUSR) || !(info.st_mode & S_IXUSR))
			{
				std::string serverName = !server_conf.getServerName().empty() ? server_conf.getServerName()[0] : "Unknown Server";
				error("Error: Insufficient permissions on root directory / server name -> " + serverName);
			}
			return;
		}
		else
		{
			std::string serverName = !server_conf.getServerName().empty() ? server_conf.getServerName()[0] : "Unknown Server";
			error("Error: The root path is not a directory / server name -> " + serverName);
		}
	}
}

void Server::check_server_name(ServerConfiguration server_conf)
{
	std::vector<std::string> serverName_vector = server_conf.getServerName();
	std::vector<std::string>::iterator serverName_it = serverName_vector.begin();
	for (; serverName_it != serverName_vector.end(); serverName_it++)
	{
		std::string serverName = *serverName_it;

		if (serverName.empty())
		{
			return;
		}

		if (serverName.length() > 253)
		{
			error("Error: Server name exceeds maximum length (253 characters allowed).");
		}

		for (size_t i; i < serverName.length(); i++)
		{
			char c = serverName[i];
			if (!isalnum(c) && c != '-' && c != '.')
			{
				error("Error : Server name contains invalid characters. It must contains alphanumeric characters and '-' / '.' only.");
			}
		}

		if (serverName[0] == '-' || serverName[serverName.length() - 1] == '-')
		{
			error("Error : Server name can not start or end with '-'.");
		}

		std::stringstream ss(serverName);
		std::string label;
		while (std::getline(ss, label, '.'))
		{
			if (label.length() > 63)
			{
				error("Error: Each label in the server name must not exceed 63 characters.");
			}
		}
	}
}

void Server::check_parsing()
{
	std::vector<ServerConfiguration> current_tab_serv = this->tab_serv;
	std::vector<ServerConfiguration>::iterator iterator_tab_serv = current_tab_serv.begin();
	for (; iterator_tab_serv != current_tab_serv.end(); iterator_tab_serv++)
	{
		check_server_name(*iterator_tab_serv);
		check_root(*iterator_tab_serv);
		check_error_page(*iterator_tab_serv);
		check_host_page(*iterator_tab_serv);
		check_index(*iterator_tab_serv);
		check_listen(*iterator_tab_serv);
		check_location(*iterator_tab_serv);
		check_max_body(*iterator_tab_serv);
	}
}
