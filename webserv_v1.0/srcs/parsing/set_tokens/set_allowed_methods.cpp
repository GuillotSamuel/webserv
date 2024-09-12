/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_allowed_methods.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 15:38:20 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/12 16:52:50 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_set_allowed_methods(std::vector<std::string> tokens)
{
    std::vector<std::string> current_allowed_methods;
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].empty()
            || std::find(current_allowed_methods.begin(), current_allowed_methods.end(), tokens[i]) != current_allowed_methods.end())
        {
            ft_invalid_line(tokens);
        }
        else if (tokens[i] == "GET" || tokens[i] == "POST" || tokens[i] == "DELETE")
        {
            current_allowed_methods.push_back(tokens[i]);
        }
    }

	this->currentConfig->setAllowedMethods(current_allowed_methods);
}
