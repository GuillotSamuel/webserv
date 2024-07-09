/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:46:07 by sguillot          #+#    #+#             */
/*   Updated: 2024/07/09 16:13:11 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static void    ft_free_parsing(t_data *data)
{
    if (data->parsing.fd != -1)
    {
        close(data->parsing.fd);
    }
}

void    ft_free_all(t_data *data)
{
    ft_free_parsing(data);
}
