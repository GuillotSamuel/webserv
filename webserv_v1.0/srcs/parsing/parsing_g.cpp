/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_g.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 19:44:12 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/18 00:36:56 by sguillot         ###   ########.fr       */
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

    std::vector<ServerConfiguration>::iterator it = this->tab_serv.begin();
    std::vector<ServerConfiguration>::iterator end = this->tab_serv.end();

    while (it != end)
    {
        std::cout << *it << std::endl;
        ++it;
    }

    check_parsing();
    std::cout << "server size : " << this->tab_serv.size() << std::endl;
}
