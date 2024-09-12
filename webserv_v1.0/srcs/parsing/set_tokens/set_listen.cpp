/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_listen.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <mmahfoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:21:41 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/12 13:30:55 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_set_listen_param(std::vector<std::string> tokens)
{
	if (tokens.size() != 2)
	{
		ft_invalid_line(tokens);
	}

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
	long port = std::strtol(num_str.c_str(), &end, 10);

	if (*end != '\0')
	{
		error("invalid argument (listen): " + str);
	}
	else if (port < 0 || port > 65535)
	{
		error("out of bounds port (listen argument): " + tokens[1]);
	}
	else if (port <= 1023					 /* Critical system 0-1023 */
			 || port == 1080					 /* SOCKS Proxy 1080 */
			 || (port >= 1433 && port <= 1434) /* Microsoft SQL server 1433-1434 */
			 || port == 1521					 /* Oracle Database 1521 */
			 || port == 1723					 /* PPTP (Point-to-Point Tunneling Protocol) 1723 */
			 || port == 3306					 /* MySQL 3306 */
			 || port == 5432					 /* PostgreSQL 5432 */
			 || (port >= 5900 && port <= 5901) /* VNC (Virtual Network Computing) 5900-5901 */
			 || port == 6379					 /* Redis 6379 */
			 || (port >= 6660 && port <= 6669) /* IRC 6660-6669 */
			 || port == 27017 				 /* MongoDB 27017 */)
	{
		error("forbiden port (listen argument): " + tokens[1]);
	}

	// std::cout << num_str << std::endl; // TEST

	this->currentConfig->setPort(num_str);
}
