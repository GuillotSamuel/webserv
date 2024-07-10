/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:57:12 by sguillot          #+#    #+#             */
/*   Updated: 2024/07/09 19:21:11 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_HPP
# define DEFINE_HPP

#define HELP_INFOS "usage : ./webserv [<file.conf] [--help]\n\n" \
    "informations required in the configuration file (token must be separated by spaces or tabulations):\n" \
    "server {\n" \
    "\tlisten <port number>\n" \
    "\tserver_name <server name>\n" \
    "\t error_page <error code> <error html file>\n" \
    "\t client_max_body_size <size in M>\n" \
    "\tlocation /<page name> {\n\t\tallow_methods <methods allowed>\n\t\tautoindex <on/off>\n\t}" \
    "}"

#endif