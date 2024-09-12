/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_max_body.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <mmahfoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:21:34 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/12 13:43:00 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_set_client_max_body_param(std::vector<std::string> tokens)
{
    if (tokens.size() != 2)
    {
        return (ft_invalid_line(tokens));
    }

    std::string str = tokens[1];

    if (!str.empty() && str[str.length() - 1] == ';')
    {
        str.erase(str.length() - 1, 1);
    }
    else
    {
        error("Error: invalid argument (max body): " + tokens[1]);
        return;
    }

    // std::cout << str << std::endl; // TEST

    this->currentConfig->setClientMaxBodySize(str);
}
