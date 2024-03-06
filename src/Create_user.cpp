/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Create_user.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaainouc <yaainouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 16:21:35 by yaainouc          #+#    #+#             */
/*   Updated: 2024/03/05 16:21:36 by yaainouc         ###   ########.fr       */
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

void create_user(int socket, std::string cmd)
{

    const char * string1 = cmd.c_str();
    std::vector<std::string> parts = splitString(string1);
    User user1(socket);
    if(parts[3] == "test")
    {
    user1.set_nickname(parts[5]);
    user1.set_username(parts[7]);   
    user1.set_hostname(parts[8]);
    user1.set_servername(parts[9]);
    user1.set_realname(parts[10]);
    }
    user1.show_userinfo();
}
