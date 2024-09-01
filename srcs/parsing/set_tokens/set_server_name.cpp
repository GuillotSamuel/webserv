/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_server_name.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:21:31 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/01 21:44:05 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void ServerConfiguration::ft_set_server_name_param(std::vector<std::string> tokens)
{
    if (tokens.size() != 2)
    {
        return (ft_invalid_line(tokens));
    }
    // else if (!this->serverName.empty())
    // {
    //     return (ft_error(data, "Server name defined many times in configurations : "
    //         + tokens[0] + " " + tokens[1]));
    // }

    const std::string& str = tokens[1];

    if (str.empty() || str[str.size() - 1] != ';')
    {
        error("invalid argument (listen): " + tokens[1]);
    }
}
