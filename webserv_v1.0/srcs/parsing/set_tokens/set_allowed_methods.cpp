/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_allowed_methods.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 15:38:20 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/18 00:26:44 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_set_allowed_methods(std::vector<std::string> tokens)
{
	for (size_t i = 1; i < tokens.size(); i++)
	{
		if (tokens[i].empty() || (tokens[i] != "GET" && tokens[i] != "POST" && tokens[i] != "DELETE") || this->location_started)
		{
			ft_invalid_line(tokens);
		}
		else
		{
			if (tokens[i] == "GET")
			{
				this->currentConfig->setAllowedMethods("GET", 1);
			}
			else if (tokens[i] == "POST")
			{
				this->currentConfig->setAllowedMethods("POST", 1);
			}
			else if (tokens[i] == "DELETE")
			{
				this->currentConfig->setAllowedMethods("DELETE", 1);
			}
		}
	}
}
