/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_g.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 19:44:12 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/04 18:44:34 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::parsing_g(int argc, char **argv)
{
    if (argc == 2 && !ft_strcmp((const char *)argv[1], (const char *)"--help"))
    {
        error(HELP_INFOS);
    }
    else if (argc == 1)
    {
        readConfigurationFile("./config/default.conf");
    }
    else if (argc == 2)
    {
        readConfigurationFile(argv[1]);
    }
    else
    {
        error("Error: args required : [./webserv] [./webserv <file.conf>] [./webserv --help]");
    }
}
