/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:56:07 by sguillot          #+#    #+#             */
/*   Updated: 2024/07/10 14:38:35 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_HPP
# define STRUCT_HPP

typedef struct s_parsing
{
    int fd;
}   t_parsing;

typedef struct s_data
{
    t_parsing   parsing;
}   t_data;

#endif