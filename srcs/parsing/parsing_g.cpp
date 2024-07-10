/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_g.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 14:00:57 by sguillot          #+#    #+#             */
/*   Updated: 2024/07/10 14:46:30 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

bool    ft_read_configuration_file(t_data *data, const char *arg)
{
    int         fd = open(arg, O_RDONLY);
    ssize_t      bytes_read = 1;
    char        buffer[1];
    std::string line;
    
    if (fd == -1)
    {
        return (ft_error(data, "Could not open the configuration file [./webserv --help]"));
    }
    
    data->parsing.fd = fd;
    
    while (bytes_read != '\0')
    {
        bytes_read = read(data->parsing.fd, buffer, 1);
        if (bytes_read == -1)
        {
            return (ft_error(data, "Error while reading from the configuration file"));
        }
        if (*buffer == '\0' || *buffer == '\n')
        {
            line += "\0";
            if (ft_serverConfiguration(data, line) == false)
                return (false);
            line = "";
        }
        else
        {
            line += *buffer;
        }
    }
    
    close(data->parsing.fd);
    return (true);
}

bool    parsing_g(t_data *data, int argc, char **argv)
{
    if (argc == 2 && !ft_strcmp((const char *)argv[1], (const char *)"--help"))
    {
        return (ft_error(data, HELP_INFOS));
    }
    else if (argc == 1)
    {
        return (ft_read_configuration_file(data, "./config/default.conf"));
    }
    else if (argc == 2)
    {
        return (ft_read_configuration_file(data, argv[1]));
    }
    else
    {
        return (ft_error(data, "args required : [./webserv] [./webserv <file.conf>] [./webserv --help]"));
    }
}
