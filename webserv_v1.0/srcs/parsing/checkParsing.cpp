/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkParsing.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 15:45:09 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/26 15:19:07 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

#include <sys/stat.h>
#include <errno.h>

void Server::check_folder(const std::string &folder_path, const std::string &server_name)
{
	struct stat info;
	char absolute_path[PATH_MAX];
	char current_dir[PATH_MAX];

	if (getcwd(current_dir, sizeof(current_dir)) == NULL)
	{
		error("Error: Cannot get current working directory: " + std::string(strerror(errno)));
	}

	if (chdir(folder_path.c_str()) != 0)
	{
		error("Error: Cannot access directory: " + std::string(strerror(errno)) +
			  " (" + folder_path + ") / server name -> " + server_name);
	}

	if (getcwd(absolute_path, sizeof(absolute_path)) == NULL)
	{
		error("Error: Cannot get absolute path: " + std::string(strerror(errno)) +
			  " (" + folder_path + ") / server name -> " + server_name);
	}

	if (chdir(current_dir) != 0)
	{
		error("Error: Cannot restore original working directory: " + std::string(strerror(errno)));
	}

	if (stat(absolute_path, &info) != 0)
	{
		error("Error: Cannot access directory: " + std::string(strerror(errno)) +
			  " (" + std::string(absolute_path) + ") / server name -> " + server_name);
	}

	if (S_ISDIR(info.st_mode))
	{
		if (!(info.st_mode & S_IRUSR) || !(info.st_mode & S_IXUSR))
		{
			error("Error: Insufficient permissions on directory: (" + std::string(absolute_path) +
				  ") / server name -> " + server_name);
		}
	}
	else
	{
		error("Error: The path is not a directory: (" + std::string(absolute_path) + ") / server name -> " + server_name);
	}
}

void Server::check_file(const std::string &folder_path, const std::string &error_page_path, const std::string &server_name)
{
	struct stat info;
	std::string full_path = folder_path + "/" + error_page_path;

	if (stat(full_path.c_str(), &info) != 0)
	{
		error("Error: Cannot access error page file: " + std::string(strerror(errno)) +
			  " (" + full_path + ") / server name -> " + server_name);
	}

	if (!S_ISREG(info.st_mode))
	{
		error("Error: The error page path is not a regular file: (" + full_path + ") / server name -> " + server_name);
	}

	if (!(info.st_mode & S_IRUSR))
	{
		error("Error: Insufficient permissions to read the error page file: (" + full_path + ") / server name -> " + server_name);
	}
}

void Server::check_error_code(int error_code, const std::string &server_name)
{
	int valid_error_codes_array[] = {400, 401, 403, 404, 405, 408, 410, 413, 414, 429,
									 500, 501, 502, 503, 504, 505};

	std::set<int> valid_error_codes(valid_error_codes_array, valid_error_codes_array + sizeof(valid_error_codes_array) / sizeof(int));

	if (valid_error_codes.find(error_code) == valid_error_codes.end())
	{
		std::stringstream ss;
		ss << "Error: Invalid error code: " << error_code << " / server name -> " << server_name;
		error(ss.str());
	}
}

void Server::check_error_page(ServerConfiguration server_conf)
{
	std::string serverName = !server_conf.getServerName().empty() ? server_conf.getServerName()[0] : "Unknown Server";

	if (!server_conf.getErrorPageLocation().empty())
	{
		check_folder(server_conf.getErrorPageLocation(), serverName);
	}

	if (!server_conf.getErrorPages().empty())
	{
		std::map<int, std::string> errorPage_map = server_conf.getErrorPages();
		std::map<int, std::string>::iterator errorPage_it = errorPage_map.begin();
		for (; errorPage_it != errorPage_map.end(); ++errorPage_it)
		{
			int error_code = errorPage_it->first;
			const std::string &error_page_path = errorPage_it->second;

			check_error_code(error_code, serverName);
			check_file(server_conf.getErrorPageLocation(), error_page_path, serverName);
		}
	}
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
	char absolute_path[PATH_MAX];

	if (!server_conf.getRoot().empty())
	{
		char current_dir[PATH_MAX];
		if (getcwd(current_dir, sizeof(current_dir)) == NULL)
		{
			error("Error: Cannot get current working directory: " + std::string(strerror(errno)));
		}

		if (chdir(server_conf.getRoot().c_str()) != 0)
		{
			std::string serverName = !server_conf.getServerName().empty() ? server_conf.getServerName()[0] : "Unknown Server";
			error("Error: Cannot access root directory: " + std::string(strerror(errno)) +
				  " (" + server_conf.getRoot() + ") / server name -> " + serverName);
		}

		if (getcwd(absolute_path, sizeof(absolute_path)) == NULL)
		{
			std::string serverName = !server_conf.getServerName().empty() ? server_conf.getServerName()[0] : "Unknown Server";
			error("Error: Cannot get absolute path: " + std::string(strerror(errno)) +
				  " / server name -> " + serverName);
		}

		if (chdir(current_dir) != 0)
		{
			error("Error: Cannot restore original working directory: " + std::string(strerror(errno)));
		}

		if (stat(absolute_path, &info) != 0)
		{
			std::string serverName = !server_conf.getServerName().empty() ? server_conf.getServerName()[0] : "Unknown Server";
			error("Error: Cannot access root directory: " + std::string(strerror(errno)) +
				  " (" + std::string(absolute_path) + ") / server name -> " + serverName);
		}

		if (S_ISDIR(info.st_mode))
		{
			if (!(info.st_mode & S_IRUSR) || !(info.st_mode & S_IXUSR))
			{
				std::string serverName = !server_conf.getServerName().empty() ? server_conf.getServerName()[0] : "Unknown Server";
				error("Error: Insufficient permissions on root directory / server name -> " + serverName);
			}
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

void Server::check_path_cgi(ServerConfiguration server_conf)
{
	(void)server_conf;
}

void Server::check_interpreter_map(ServerConfiguration server_conf)
{
	(void)server_conf;
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
		check_max_body(*iterator_tab_serv);
		check_path_cgi(*iterator_tab_serv);
		check_interpreter_map(*iterator_tab_serv);
		check_location(*iterator_tab_serv);
	}
}
