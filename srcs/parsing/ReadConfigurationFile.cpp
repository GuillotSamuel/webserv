/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReadConfigurationFile.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 20:01:59 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/01 20:12:20 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void ServerConfiguration::readConfigurationFile(const char *arg)
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
}
