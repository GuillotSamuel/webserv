/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_tokens.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 16:09:51 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/01 21:22:10 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

// bool ft_invalid_line(t_data *data, std::vector<std::string> tokens)
// {
//     std::string error_message = "invalid line (no lines allowed after server configurations) [./webserv --help]: ";
//     for (int i = 0; (long unsigned int)i < tokens.size(); i++)
//     {
//         error_message += tokens[i];
//         error_message += " ";
//     }
//     return (ft_error(data, error_message));
// }

// bool ft_param_set_tokens(t_data *data, std::vector<std::string> tokens)
// {
//     const int nb_param = 6;
    
//     // Use fixed-size arrays and manual initialization
//     const char* param_array[nb_param] = {"listen", "host_name", "server_name", "error_page",
//                                          "client_max_body", "location"};
    
//     // Fixed-size array of function pointers
//     bool (*param_functions[nb_param])(t_data *, const std::vector<std::string>) = {
//         &ft_set_listen_param,
//         &ft_set_host_name_param,
//         &ft_set_server_name_param,
//         &ft_set_error_page_param,
//         &ft_set_client_max_body_param,
//         &ft_set_location_param
//     };

//     for (int i = 0; i < nb_param; i++)
//     {
//         if (tokens.size() > 0 && std::strcmp(tokens[0].c_str(), param_array[i]) == 0)
//         {
//             return param_functions[i](data, tokens);
//         }
//     }

//     std::string error_message = "unknown parameter: " + (tokens.size() > 0 ? tokens[0] : "");
//     return ft_error(data, error_message);
// }

// bool ft_start_set_tokens(t_data *data, std::vector<std::string> tokens)
// {
//         if (tokens.size() == 1 && data->parsing.parsing_started_server == false && tokens[0] == "server")
//         {
//             data->parsing.parsing_started_server = true;
//         }
//         else if ((tokens.size() == 1 && data->parsing.parsing_started_server == false && tokens[0] == "server{")
//             || (tokens.size() == 2 && data->parsing.parsing_started_server == false && tokens[0] == "server" && tokens[1] == "{"))
//         {
//             data->parsing.parsing_started_server = true;
//             data->parsing.parsing_started_brace = true;
//         }
//         else if (tokens.size() == 1 && data->parsing.parsing_started_server == true && data->parsing.parsing_started_brace == false && tokens[0] == "{")
//         {
//             data->parsing.parsing_started_brace = true;
//         }
//         else
//         {
//             std::string error_message = "invalid line [./webserv --help]: ";
//             for (int i = 0; (long unsigned int)i < tokens.size(); i++)
//             {
//                 error_message += tokens[i];
//                 error_message += " ";
//             }
//             return (ft_error(data, error_message));
//         }
//         return (true);
// }

// bool    ft_set_tokens(t_data *data, std::vector<std::string> tokens)
// {
//     /* START OF TEST */
//     std::string test;
//     for (int i = 0; (long unsigned int)i < tokens.size(); i++)
//     {
//         test += tokens[i];
//         test += "|";
//     }
//     std::cout << test << std::endl;
//     /* END OF TEST */
    
//     if (!tokens.empty())
//     {
//         if (data->parsing.parsing_ended == true)
//         {
//             return (ft_invalid_line(data, tokens));
//         }
//         else if ((data->parsing.parsing_started_server == false || data->parsing.parsing_started_brace == false))
//         {
//             return (ft_start_set_tokens(data, tokens));
//         }
//         else if ((data->parsing.parsing_started_server == true && data->parsing.parsing_started_brace == true) && tokens.size() >= 1 && tokens[0] != "}")
//         {
//             return (ft_param_set_tokens(data, tokens));
//         }
//         else if ((data->parsing.parsing_started_server == true && data->parsing.parsing_started_brace == true) && tokens.size() >= 1 && tokens[0] == "}")
//         {
//             data->parsing.parsing_ended = true;
//             return (true);
//         }
//     }
//     return (true);
// }

