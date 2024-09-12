/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_root.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:06:46 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/12 15:47:47 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_set_root_param(std::vector<std::string> tokens)
{
    if (tokens.size() != 2 || tokens[1].empty())
    {
        ft_invalid_line(tokens);
    }

    this->currentConfig->setRoot(tokens[1]);
}

    
