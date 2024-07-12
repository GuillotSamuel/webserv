/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:51:56 by sguillot          #+#    #+#             */
/*   Updated: 2024/07/12 18:51:03 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

/* -------------------------------------------------------------------------- */
/*                                    DEFINE                                  */
/* -------------------------------------------------------------------------- */

#define HELP_INFOS "usage : ./webserv [<file.conf] [--help]\n\n" \
    "informations required in the configuration file (token must be separated by spaces or tabulations):\n\n" \
    "server {\n" \
    "\tlisten <port number>\n\n" \
    "\tserver_name <server name>\n\n" \
    "\t error_page <error code> <error html file>\n\n" \
    "\t client_max_body_size <size in M>\n\n" \
    "\tlocation /<page name> {\n\t\tallow_methods <methods allowed>\n\t\tautoindex <on/off>\n\t}\n" \
    "}"

/* -------------------------------------------------------------------------- */
/*                                    INCLUDES                                */
/* -------------------------------------------------------------------------- */

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

#include "serverConfiguration.hpp"

/* -------------------------------------------------------------------------- */
/*                                    STRUCTURES                              */
/* -------------------------------------------------------------------------- */

typedef struct s_parsing
{
    int     fd;
    bool    parsing_started_server;
    bool    parsing_started_brace;
    bool    parsing_ended;
}   t_parsing;

typedef struct s_data
{
    ServerConfiguration *p_server_configuration;
    t_parsing   parsing;
}   t_data;

/* -------------------------------------------------------------------------- */
/*                                    UTILS                                   */
/* -------------------------------------------------------------------------- */

/* error.cpp */
bool    ft_error(t_data *data, std::string str);

/* free_all.cpp */
void    ft_free_all(t_data *data);

/* init.cpp */
void    ft_init(t_data *data, ServerConfiguration *server_configuration);

/* utils.cpp */
int     ft_strcmp(const char *s1, const char *s2);

/* -------------------------------------------------------------------------- */
/*                                    PARSING                                 */
/* -------------------------------------------------------------------------- */

/* parsing_g.cpp */
bool    parsing_g(t_data *data, int argc, char **argv);

/* read_configuration.cpp */
bool    ft_read_configuration_file(t_data *data, const char *arg);

/* set_tokens.cpp */
bool    ft_set_tokens(t_data *data, std::vector<std::string> tokens);

/* tokenizer.cpp */
bool    ft_tokenizer(t_data *data, const std::string &line);

/* -------------------------------------------------------------------------- */
/*                                    EXEC                                    */
/* -------------------------------------------------------------------------- */

/* exec_g.cpp */
bool    exec_g(t_data *data);

#endif