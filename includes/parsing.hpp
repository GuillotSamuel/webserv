/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 14:30:30 by sguillot          #+#    #+#             */
/*   Updated: 2024/07/10 11:50:04 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
# define PARSING_HPP

#include "struct.hpp"

/* parsing_g.cpp */
void    parsing_g(t_data *data, int argc, char **argv);

/* serverConfiguration.cpp */
void    ft_serverConfiguration(t_data *data, const std::string &line);

#endif