/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReadConfigurationFile.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 20:01:59 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/21 15:19:16 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::readConfigurationFile(const char *arg)
{
	int fd = open(arg, O_RDONLY); 
	if (fd == -1)
	{
		error("Error: Could not open the configuration file [./webserv --help]");
	}

	/*std::ifstream configFile(arg); // utiliser plutot une fonction ccp
	if (configFile.fail())
	{
		error("Error: Could not open the configuration file [./webserv --help]");
	}*/

	ssize_t bytes_read = 1;
	char buffer[1];
	std::string line;
	this->currentConfig = NULL;

	this->insideServerBlock = false;
	this->insideParamBlock = false;
	this->fd_config = fd;

	// //voir pour utiliser getline() // lis automatiquement jusqu'a newline
	/*while (getline(configFile, line))
	{
		int i = 0;
		for (i = 0; std::isspace(line[i]); i++);
		if (line[i] != '#' && !line.empty())
		{
			if (this->insideServerBlock)
			{
				if (this->insideParamBlock == false && line.find("}") != std::string::npos)
				{
					this->insideServerBlock = false;
					this->tab_serv.push_back(*this->currentConfig);
					delete this->currentConfig;
					this->currentConfig = NULL;
				}
				else if (this->insideParamBlock == true && line.find("}") != std::string::npos)
				{
					this->insideParamBlock = false;
					ft_tokenizer(line);
				}
				else if (this->insideParamBlock == false && line.find("{") != std::string::npos)
				{
					this->insideParamBlock = true;
					ft_tokenizer(line);
				}
				else
				{
					ft_tokenizer(line);
				}
			}
			else
			{
				if (line.find("server {") != std::string::npos)
				{
					this->currentConfig = new ServerConfiguration();
					this->location_started = false;
					if (!this->currentConfig) // inutile sauf si tu precise a new que tu veux pas d'exception (new(std::nothrow))
					{
						error("Error: Unable to allocate memory for ServerConfiguration");
					}
					insideServerBlock = true;
				}
			}
			line.clear();
		}
		line.clear();
	}*/
	while ((bytes_read = read(this->fd_config, buffer, 1)) > 0) // pareil ici plutot cpp
	{
		if (bytes_read == -1)
		{
			error("Error: Error while reading from the configuration file");
		}

		if (*buffer == '\0' || *buffer == '\n')
		{
			size_t start = line.find_first_not_of(" \t");
			if (start != std::string::npos)
			{
				line = line.substr(start);
			}

			size_t comment_pos = line.find('#');
			if (comment_pos != std::string::npos)
			{
				line = line.substr(0, comment_pos);
			}

			if (line.empty())
			{
				line.clear();
				continue;
			}
			
			if (this->insideServerBlock)
			{
				if (this->insideParamBlock == false && line.find("}") != std::string::npos)
				{
					this->insideServerBlock = false;
					this->tab_serv.push_back(*this->currentConfig);
					delete this->currentConfig;
					this->currentConfig = NULL;
				}
				else if (this->insideParamBlock == true && line.find("}") != std::string::npos)
				{
					this->insideParamBlock = false;
					ft_tokenizer(line);
				}
				else if (this->insideParamBlock == false && line.find("{") != std::string::npos)
				{
					this->insideParamBlock = true;
					ft_tokenizer(line);
				}
				else
				{
					ft_tokenizer(line);
				}
			}
			else
			{
				if (line.find("server {") != std::string::npos)
				{
					this->currentConfig = new ServerConfiguration();
					this->location_started = false;
					if (!this->currentConfig)
					{
						error("Error: Unable to allocate memory for ServerConfiguration");
					}
					insideServerBlock = true;
				}
			}
			line.clear();
		}
		else
		{
			line += *buffer;
		}
	}

	if (insideServerBlock && this->currentConfig != NULL)
	{
		this->tab_serv.push_back(*this->currentConfig);
		delete this->currentConfig;
		this->currentConfig = NULL;
	}

	close(this->fd_config);
}
