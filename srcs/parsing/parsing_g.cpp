/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_g.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 14:00:57 by sguillot          #+#    #+#             */
/*   Updated: 2024/07/09 19:30:01 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void    parsing_g(t_data *data, int argc, char **argv)
{
    if (argc != 2)
    {
        ft_error(data, "args required : ./webserv <file.conf> (or --help)");
    }

    if (!ft_strcmp((const char *)argv[1], (const char *)"--help"))
    {
        ft_error(data, HELP_INFOS);
    }
    
    int         fd = open(argv[1], O_RDONLY);
    ssize_t      bytes_read = 1;
    char        buffer[1];
    std::string line;
    if (fd == -1)
    {
        ft_error(data, "Could not open the configuration file");
    }
    data->parsing.fd = fd;
    while (bytes_read != '\0')
    {
        bytes_read = read(data->parsing.fd, buffer, 1);
        if (bytes_read == -1)
        {
            ft_error(data, "Error while reading from the configuration file");
        }
        if (*buffer == '\0' || *buffer == '\n')
        {
            ft_serverConfiguration(data, line);
            line = "";
        }
        else
        {
            line += *buffer;
        }
    }
    close(data->parsing.fd);
}
