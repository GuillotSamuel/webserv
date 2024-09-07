/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:31:35 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/06 11:46:05 by mmahfoud         ###   ########.fr       */
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
	
	std::vector<ServerConfiguration> _serv;
	ServerConfiguration *serv1 = new ServerConfiguration("8090");
	ServerConfiguration *serv2 = new ServerConfiguration("8091");

	_serv.push_back(*serv1);
	_serv.push_back(*serv2);

	try
	{
		Server server_object(_serv);
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