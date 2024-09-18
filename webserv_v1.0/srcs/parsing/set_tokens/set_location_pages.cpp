/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_location_pages.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:15:09 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/18 16:11:49 by sguillot         ###   ########.fr       */
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
	else if (*current_param_it == "allowed_methods" && current_param.size() >= 2)
	{
		for (size_t i = 1; i < current_param.size(); i++)
		{
			if (current_param[i].empty() || (current_param[i] != "GET" && current_param[i] != "POST" && current_param[i] != "DELETE"))
			{
				ft_invalid_line(current_param);
			}
			else
			{
				if (current_param[i] == "GET")
				{
					new_location.allowed_methods["GET"] = 1;
				}
				else if (current_param[i] == "POST")
				{
					new_location.allowed_methods["POST"] = 1;
				}
				else if (current_param[i] == "DELETE")
				{
					new_location.allowed_methods["DELETE"] = 1;
				}
			}
		}
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

	new_location.allowed_methods["GET"] = 0;
	new_location.allowed_methods["POST"] = 0;
	new_location.allowed_methods["DELETE"] = 0;

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

		this->currentConfig->setLocationMap(location_key, new_location);
	}
}
