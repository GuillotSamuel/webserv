/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_g.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 14:00:57 by sguillot          #+#    #+#             */
/*   Updated: 2024/07/09 16:22:58 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void    parsing_g(t_data *data, int argc, char **argv)
{
    if (argc != 2)
    {
        ft_error(data, "args required : ./webserv <file.conf>");
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
            ft_serverConfiguration(data, buffer);
            line = "";
        }
        else
        {
            line += *buffer;
        }
    }
    close(data->parsing.fd);
}
