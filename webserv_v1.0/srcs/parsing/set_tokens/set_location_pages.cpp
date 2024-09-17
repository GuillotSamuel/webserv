/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_location_pages.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:15:09 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/17 15:55:00 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_location_pages_dispatch(std::vector<std::string> current_param)
{
	std::vector<std::string>::iterator current_param_it = current_param.begin();

	if (*current_param_it == "alias")
	{
		
	}
	else if (*current_param_it == "root")
	{
		
	}
	else if (*current_param_it == "client_max_body_size")
	{
		
	}
	else if (*current_param_it == "auto_index")
	{
		
	}
	else if (*current_param_it == "path_info")
	{
		
	}
	else if (*current_param_it == "index")
	{
		
	}
	else if (*current_param_it == "uploads_location")
	{
		
	}
	else if (*current_param_it == "allowed_methods")
	{
		
	}
	
}

void Server::ft_location_pages(std::vector<std::string> tokens)
{
	std::vector<std::string> current_param;

	if (tokens.size() < 5 || tokens[2] != "|")
	{
		ft_invalid_line(tokens);
	}

	for (size_t i = 3; i < tokens.size(); i++)
	{
		current_param.clear();

		while (i < tokens.size() && tokens[i] != "|")
		{
			current_param.push_back(tokens[i]);
			i++;
		}

		if (!current_param.empty())
		{
			ft_location_pages_dispatch(current_param);
		}
	}
}
