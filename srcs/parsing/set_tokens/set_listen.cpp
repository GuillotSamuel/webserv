/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_listen.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:21:41 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/01 21:46:57 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void ServerConfiguration::ft_set_listen_param(std::vector<std::string> tokens)
{
	if (tokens.size() != 2)
	{
		ft_invalid_line(tokens);
	}
	/* else if (data->p_server_configuration->getPort() != -1) // A REVOIR
	{
		error("listen defined many times in configurations : " + tokens[0] + " " + tokens[1]);
	} */

	const std::string &str = tokens[1];

	if (str.empty() || str[str.size() - 1] != ';')
	{
		error("invalid argument (listen): " + tokens[1]);
	}

	for (size_t i = 0; i < str.size() - 1; ++i)
	{
		if (!std::isdigit(str[i]))
		{
			error("non-numeric character in port number: " + str);
		}
	}

	std::string num_str = str.substr(0, str.size() - 1); // A REVOIR
	char *end;
	long num = std::strtol(num_str.c_str(), &end, 10);

	if (*end != '\0')
	{
		error("invalid argument (listen): " + str);
	}
	else if (num < 0 || num > 65535)
	{
		error("out of bounds port (listen argument): " + tokens[1]);
	}
	else if (num <= 1023					 /* Critical system 0-1023 */
			 || num == 1080					 /* SOCKS Proxy 1080 */
			 || (num >= 1433 && num <= 1434) /* Microsoft SQL server 1433-1434 */
			 || num == 1521					 /* Oracle Database 1521 */
			 || num == 1723					 /* PPTP (Point-to-Point Tunneling Protocol) 1723 */
			 || num == 3306					 /* MySQL 3306 */
			 || num == 5432					 /* PostgreSQL 5432 */
			 || (num >= 5900 && num <= 5901) /* VNC (Virtual Network Computing) 5900-5901 */
			 || num == 6379					 /* Redis 6379 */
			 || (num >= 6660 && num <= 6669) /* IRC 6660-6669 */
			 || num == 27017 /* MongoDB 27017 */)
	{
		error("forbiden port (listen argument): " + tokens[1]);
	}

	this->port = static_cast<int>(tokens[1].substr(0, tokens[1].size() - 1));
}
