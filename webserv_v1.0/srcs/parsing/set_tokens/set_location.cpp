/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_location.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:21:38 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/12 11:54:41 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_set_location_param(std::vector<std::string> tokens)
{
    if (tokens.size() != 3)
    {
        return (ft_invalid_line(tokens));
    }

    std::string page = tokens[1];
	std::string location = tokens[2];

    if (page.empty())
    {
        error("Error: invalid argument (location): " + page);
        return;
    }

    if (!location.empty() && location[location.length() - 1] == ';')
    {
        location.erase(location.length() - 1, 1);
    }
    else
    {
        error("Error: invalid argument (location): " + tokens[2]);
        return;
    }

	this->currentConfig->setLocation(page, location);
}
