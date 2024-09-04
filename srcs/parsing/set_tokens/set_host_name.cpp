/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_host_name.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:21:44 by sguillot          #+#    #+#             */
/*   Updated: 2024/07/15 12:28:02 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static bool ft_is_ip_address(const std::string& str)
{
    if (str.size() < 7 || str.size() > 15
        || std::strncmp("10", str.c_str(), 2) == 0
        || std::strncmp("192.168", str.c_str(), 7) == 0
        || std::strncmp("0.0.0.0", str.c_str(), 7) == 0
        || std::strncmp("255.255.255.255", str.c_str(), 15) == 0)
    {
        return (false);
    }

    int num = 0;
    int point_counter = 0;
    int segment_length = 0;

    for (size_t i = 0; i < str.size(); ++i)
    {
        char ch = str[i];
        if (ch == '.')
        {
            if (num < 0 || num > 255 || segment_length == 0 || segment_length > 3)
            {
                return (false);
            }
            num = 0;
            segment_length = 0;
            ++point_counter;
        }
        else if (ch >= '0' && ch <= '9')
        {
            num = num * 10 + (ch - '0');
            ++segment_length;
        }
        else
        {
            return (false);
        }
    }

    if (num < 0 || num > 255 || point_counter != 3
        || segment_length == 0 || segment_length > 3)
    {
        return (false);
    }

    return (true);
}


bool ft_set_host_name_param(t_data *data, std::vector<std::string> tokens)
{
    if (tokens.size() != 2)
    {
        return (ft_invalid_line(data, tokens));
    }
    else if (!data->p_server_configuration->getHostName().empty())
    {
        return (ft_error(data, "Host name defined many times in configurations : "
            + tokens[0] + " " + tokens[1]));
    }

    const std::string& str = tokens[1];

    if (str.empty() || str[str.size() - 1] != ';')
    {
        return (ft_error(data, "invalid argument (listen): " + tokens[1]));
    }
    
    if (!ft_is_ip_address(tokens[1].substr(0, tokens[1].size() - 1)))
    {
        return (ft_invalid_line(data, tokens));
    }

    data->p_server_configuration->setHostName(tokens[1].substr(0, tokens[1].size() - 1));
    return (true);
}
