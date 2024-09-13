/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_index.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:07:06 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/13 11:32:20 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_set_index_param(std::vector<std::string> tokens)
{
    if (tokens.size() != 2 || tokens[1].empty())
    {
        ft_invalid_line(tokens);
    }

    this->currentConfig->setIndex(tokens[1]);
}
