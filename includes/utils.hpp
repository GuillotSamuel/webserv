/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:50:35 by sguillot          #+#    #+#             */
/*   Updated: 2024/07/10 14:46:59 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

/* error.cpp */
bool    ft_error(t_data *data, std::string str);

/* free_all.cpp */
void    ft_free_all(t_data *data);

/* init.cpp */
void    ft_init(t_data *data);

/* utils.cpp */
int     ft_strcmp(const char *s1, const char *s2);

#endif