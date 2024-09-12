/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_root.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:06:46 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/12 16:13:04 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_set_root_param(std::vector<std::string> tokens)
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
        error("Error: invalid argument (root): " + tokens[1]);
        return;
    }

    // std::cout << str << std::endl; // TEST

    this->currentConfig->setRoot(str);
}

    
