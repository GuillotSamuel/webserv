/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 15:45:21 by sguillot          #+#    #+#             */
/*   Updated: 2024/07/10 17:13:53 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static void    ft_init_parsing(t_data *data)
{
    data->parsing.fd = -1;
    data->parsing.parsing_started = false;
}

void    ft_init(t_data *data)
{
    ServerConfiguration   server_configuration(void);
    ft_init_parsing(data);
}
