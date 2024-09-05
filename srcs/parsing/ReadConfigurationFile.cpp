/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReadConfigurationFile.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 20:01:59 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/05 23:36:17 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/* void Server::readConfigurationFile(const char *arg)
{
	int fd = open(arg, O_RDONLY);
	ssize_t bytes_read = 1;
	char buffer[1];
	std::string line;

	if (fd == -1)
	{
		error("Error: Could not open the configuration file [./webserv --help]");
	}

	this->fd_config = fd;

	while ((bytes_read = read(this->fd_config, buffer, 1)) > 0)
	{
		if (bytes_read == -1)
		{
			error("Error: Error while reading from the configuration file");
		}
		if (*buffer == '\0' || *buffer == '\n')
		{
			line += "\0";
			ft_tokenizer(line);
			line = "";
		}
		else
		{
			line += *buffer;
		}
	}

	close(this->fd_config);
} */

void Server::readConfigurationFile(const char *arg)
{
	int fd = open(arg, O_RDONLY);
	if (fd == -1)
	{
		error("Error: Could not open the configuration file [./webserv --help]");
	}
	
	ssize_t bytes_read = 1;
	char buffer[1];
	std::string line;
	this->currentConfig = NULL;

	this->insideServerBlock = false;
	this->insideParamBlock = false;
	this->fd_config = fd;

	while ((bytes_read = read(this->fd_config, buffer, 1)) > 0)
	{
		if (bytes_read == -1)
        {
            error("Error: Error while reading from the configuration file");
        }
		if (*buffer == '\0' || *buffer == '\n')
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
				}
				else if (this->insideParamBlock == false && line.find("{") != std::string::npos)
				{
					this->insideParamBlock = true;
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
