/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:31:35 by sguillot          #+#    #+#             */
/*   Updated: 2024/08/29 21:27:56 by mmahfoud         ###   ########.fr       */
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
	// t_data 				data;
	

	signal(SIGINT, sighandler);
	
	try
	{
		Server server_object;
		server_object.startingServer();
		server_object.serverExecution();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
                return (false);
    }
	
	(void)argc; // TEST
	(void)argv; // TEST

	// ft_init(&data, &server_configuration);
	// if (parsing_g(&data, argc, argv) == false
	// 	|| exec_g(&data, server_configuration) == false)
	// {
	// 	return (EXIT_FAILURE);
	// }
	// std::cout << server_configuration << std::endl;
	
	
	
	return (0);
}