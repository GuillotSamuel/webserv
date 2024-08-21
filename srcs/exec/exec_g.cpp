/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_g.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 14:00:46 by sguillot          #+#    #+#             */
/*   Updated: 2024/08/21 14:06:02 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "include.hpp"


bool    exec_g(t_data *data)
{
        try
	{
		Server server_object(argv[1], envp);

	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
                return (false)
	}

	return (true);
}