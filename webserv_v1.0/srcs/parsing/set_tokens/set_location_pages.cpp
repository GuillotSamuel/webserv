/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_location_pages.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:15:09 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/17 23:00:19 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_location_pages_dispatch(std::vector<std::string> current_param, t_location &new_location)
{
	std::vector<std::string>::iterator current_param_it = current_param.begin();

	if (*current_param_it == "alias" && current_param.size() == 2 && !current_param[1].empty())
	{
		new_location.alias = current_param[1];
	}
	else if (*current_param_it == "root" && current_param.size() == 2 && !current_param[1].empty())
	{
		new_location.root = current_param[1];
	}
	else if (*current_param_it == "client_max_body_size" && current_param.size() == 2 && !current_param[1].empty())
	{
		new_location.clientMaxBodySize = current_param[1];
	}
	else if (*current_param_it == "auto_index" && current_param.size() == 2 && !current_param[1].empty())
	{
		if (current_param[1] == "on")
			new_location.autoindex = 1;
		else if (current_param[1] == "off")
			new_location.autoindex = 0;
		else
		{
			ft_invalid_line(current_param);
		}	
	}
	else if (*current_param_it == "path_info" && current_param.size() == 2 && !current_param[1].empty())
	{
		new_location.path_info = current_param[1];
	}
	else if (*current_param_it == "index" && current_param.size() == 2 && !current_param[1].empty())
	{
		new_location.index = current_param[1];
	}
	else if (*current_param_it == "uploads_location" && current_param.size() == 2 && !current_param[1].empty())
	{
		new_location.uploadsLocation = current_param[1];
	}
	else if (*current_param_it == "allowed_methods")
	{
		
	}
	else
	{
		ft_invalid_line(current_param);
	}
}

void Server::ft_location_pages(std::vector<std::string> tokens)
{
	std::vector<std::string> current_param;
	t_location new_location;

	if (tokens.size() < 5 || tokens[2] != "|")
	{
		ft_invalid_line(tokens);
	}

	std::string location_key = tokens[1];

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
			ft_location_pages_dispatch(current_param, new_location);
		}
		
		// this->currentConfig->_locations_map[location_key] = new_location;
		this->currentConfig->setLocationMap(location_key, new_location);
	}
}
