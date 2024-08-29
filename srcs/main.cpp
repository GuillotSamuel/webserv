/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:31:35 by sguillot          #+#    #+#             */
/*   Updated: 2024/08/29 12:28:07 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	main(int argc, char **argv)
{
	// t_data 				data;
	ServerConfiguration	*serv;


	serv = new ServerConfiguration();
	
	(void)argc; // TEST
	(void)argv; // TEST

	// ft_init(&data, &server_configuration);
	// if (parsing_g(&data, argc, argv) == false
	// 	|| exec_g(&data, server_configuration) == false)
	// {
	// 	return (EXIT_FAILURE);
	// }
	// std::cout << server_configuration << std::endl;
	
	exec_g(serv);
	
	return (0);
}