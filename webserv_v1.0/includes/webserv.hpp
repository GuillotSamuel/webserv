/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:51:56 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/28 17:08:17 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/* -------------------------------------------------------------------------- */
/*                                    INCLUDES                                */
/* -------------------------------------------------------------------------- */

#include <iostream>
#include <vector>
#include <cstring>
#include <functional>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <list>
#include <map>
#include <string_view>
#include <ctime>
#include <set>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <signal.h>
#include <limits.h>
#include <netdb.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

/* -------------------------------------------------------------------------- */
/*                                    DEFINE                                  */
/* -------------------------------------------------------------------------- */

#define MAX_EVENTS 10
#define BUFFER_SIZE 4096
#define SIGNAL 2
#define RESET "\033[0m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define RED "\033[31m"
#define BOLD "\033[1m"
#define HELP_INFOS "usage : ./webserv [<file.conf] [--help]\n\n" \
    "informations required in the configuration file (token must be separated by spaces or tabulations):\n\n" \
    "server {\n" \
    "\tlisten <port number>\n\n" \
    "\tserver_name <server name>\n\n" \
    "\t error_page <error code> <error html file>\n\n" \
    "\t client_max_body_size <size in M>\n\n" \
    "\tlocation /<page name> {\n\t\tallow_methods <methods allowed>\n\t\tautoindex <on/off>\n\t}\n" \
    "}"

extern int g_signal;

/* -------------------------------------------------------------------------- */
/*                                FORWARD DECLARATIONS                        */
/* -------------------------------------------------------------------------- */


#include "Location.hpp"
#include "Server.hpp"
#include "ServerConfiguration.hpp"
#include "listeningSocket.hpp"
#include "Client.hpp"
#include "Cgi.hpp"
#include "Response.hpp"

/* -------------------------------------------------------------------------- */
/*                                    UTILS                                   */
/* -------------------------------------------------------------------------- */

/* utils.cpp */
int     ft_strcmp(const char *s1, const char *s2);
