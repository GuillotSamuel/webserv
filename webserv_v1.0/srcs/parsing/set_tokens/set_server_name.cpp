/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_server_name.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:21:31 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/13 11:40:43 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_set_server_name_param(std::vector<std::string> tokens)
{
    if (tokens.size() != 2 || tokens[1].empty())
    {
        ft_invalid_line(tokens);
    }

    this->currentConfig->setServerName(tokens[1]);
}