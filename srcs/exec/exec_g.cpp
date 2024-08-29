/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_g.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 14:00:46 by sguillot          #+#    #+#             */
/*   Updated: 2024/08/29 12:28:25 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

bool    exec_g(ServerConfiguration *inf)
{
	// (void)data;
    try
	{
		Server server_object(*inf);

	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
                return (false);
        }
        
    return (true);
}