/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:31:35 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/23 14:57:49 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"


int	g_signal = 0;

void sighandler(int type)
{
	(void)type;
	g_signal = SIGNAL;
}


int	main(int argc, char **argv)
{
	signal(SIGINT, sighandler);
	try
	{
		Server server_object(argc, argv);
		// server_object.startingServer();
		// server_object.serverExecution();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
        return (false);
    }
	
	return (0);
}