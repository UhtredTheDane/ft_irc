/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Create_user.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaainouc <yaainouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 16:21:35 by yaainouc          #+#    #+#             */
/*   Updated: 2024/03/15 03:14:15 by agengemb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/User.hpp"

std::vector<std::string> splitString(const char in[])
{
    std::istringstream iss(in);
    std::istream_iterator<std::string> first(iss), last;

    std::vector<std::string> parts;
    std::copy(first, last, std::back_inserter(parts));
    return parts;
}
/*
    user1.set_realname(parts[10]);
    std::string msg = user1.get_servername() + " ";
    msg += "003 ";
    msg += user1.get_nickname();
    msg += " :Welcome to our irc server ";
    msg += user1.get_identifier();
    msg += "\r\n";
    int msg_len = msg.length();
    user1.show_userinfo();
    send(socket, msg.c_str(), msg_len, 0);
    
*/
