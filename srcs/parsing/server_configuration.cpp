/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_configuration.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:36:24 by sguillot          #+#    #+#             */
/*   Updated: 2024/07/10 11:49:18 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void ft_serverConfiguration(t_data *data, const std::string &line)
{
    (void)data; // TO REMOVE

    std::vector<std::string>    tokens;
    std::string                 buffer;
    std::string::const_iterator line_copy;
    
    for (line_copy = line.begin(); line_copy != line.end(); ++line_copy)
    {
        if (!std::isspace(*line_copy))
        {
            buffer += *line_copy;
        }
        else if (!buffer.empty())
        {
            tokens.push_back(buffer);
            buffer.clear();
        }
    }
    
    if (!buffer.empty())
    {
        tokens.push_back(buffer);
    }
}

