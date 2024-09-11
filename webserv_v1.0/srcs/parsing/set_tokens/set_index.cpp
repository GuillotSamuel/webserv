/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_index.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:07:06 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/11 18:51:19 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_set_index_param(std::vector<std::string> tokens)
{
	if (tokens.size() != 2)
    {
        return (ft_invalid_line(tokens));
    }

    const std::string& str = tokens[1];

    if (str.empty() || str[str.size() - 1] != ';')
    {
        error("Error: invalid argument (index): " + tokens[1]);
    }
    
    this->currentConfig->setIndex(tokens[1]);
}
