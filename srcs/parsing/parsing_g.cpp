/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_g.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 14:00:57 by sguillot          #+#    #+#             */
/*   Updated: 2024/08/20 20:03:03 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

bool    parsing_g(t_data *data, int argc, char **argv)
{
    if (argc == 2 && !ft_strcmp((const char *)argv[1], (const char *)"--help"))
    {
        return (ft_error(data, HELP_INFOS));
    }
    else if (argc == 1)
    {
        return (readConfigurationFile(data, "./config/default.conf"));
    }
    else if (argc == 2)
    {
        return (readConfigurationFile(data, argv[1]));
    }
    else
    {
        return (ft_error(data, "args required : [./webserv] [./webserv <file.conf>] [./webserv --help]"));
    }
}
