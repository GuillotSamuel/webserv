/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_listen.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:21:41 by sguillot          #+#    #+#             */
/*   Updated: 2024/07/15 12:23:55 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

bool ft_set_listen_param(t_data *data, std::vector<std::string> tokens)
{
    if (tokens.size() != 2)
    {
        return (ft_invalid_line(data, tokens));
    }
    else if (data->p_server_configuration->getPort() != -1)
    {
        return (ft_error(data, "listen defined many times in configurations : "
            + tokens[0] + " " + tokens[1]));
    }
    
    const std::string& str = tokens[1];

    if (str.empty() || str[str.size() - 1] != ';')
    {
        return ft_error(data, "invalid argument (listen): " + tokens[1]);
    }

    for (size_t i = 0; i < str.size() - 1; ++i)
    {
        if (!std::isdigit(str[i]))
        {
            return ft_error(data, "non-numeric character in port number: " + str);
        }
    }

    const char* num_str = str.substr(0, str.size() - 1).c_str();
    char* end;
    long num = std::strtol(num_str, &end, 10);
    
    if (*end != '\0')
    {
        return ft_error(data, "invalid argument (listen): " + str);
    }
    else if (num < 0 || num > 65535)
    {
        return ft_error(data, "out of bounds port (listen argument): " + tokens[1]);
    }
    else if (num <= 1023 /* Critical system 0-1023 */
        || num == 1080 /* SOCKS Proxy 1080 */
        || (num >= 1433 && num <= 1434) /* Microsoft SQL server 1433-1434 */
        || num == 1521 /* Oracle Database 1521 */
        || num == 1723 /* PPTP (Point-to-Point Tunneling Protocol) 1723 */
        || num == 3306 /* MySQL 3306 */
        || num == 5432 /* PostgreSQL 5432 */
        || (num >= 5900 && num <= 5901) /* VNC (Virtual Network Computing) 5900-5901 */
        || num == 6379 /* Redis 6379 */
        || (num >= 6660 && num <= 6669) /* IRC 6660-6669 */
        || num == 27017 /* MongoDB 27017 */)
    {
        return (ft_error(data, "forbiden port (listen argument): " + tokens[1]));
    }
    
    data->p_server_configuration->setPort(tokens[1].substr(0, tokens[1].size() - 1));
    return (true);
}
