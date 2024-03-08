/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Create_user.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaainouc <yaainouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 16:21:35 by yaainouc          #+#    #+#             */
/*   Updated: 2024/03/07 18:13:27 by yaainouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/User.hpp"
#include "stdio.h"
std::vector<std::string> splitString(const char in[])
{
    std::istringstream iss(in);
    std::istream_iterator<std::string> first(iss), last;

    std::vector<std::string> parts;
    std::copy(first, last, std::back_inserter(parts));
    return parts;
}

void create_user(int socket, std::string cmd, std::vector<pollfd> *poll_fds)
{
    (void)poll_fds;
    (void)socket;
    // const char *msg = "";
    // int msg_len = strlen(msg);
    const char * string1 = cmd.c_str();
    std::vector<std::string> parts = splitString(string1);
    User user1(socket);
    Server_msg reply;
    if(parts[3] == "test")
    {
    User user1(socket);
    user1.set_nickname(parts[5]);
    user1.set_username(parts[7]);   
    user1.set_hostname(parts[8]);
    user1.set_servername(parts[9]);
    user1.set_realname(parts[10]);
    // std::string msg = ":irc.42.com ";
    // int msg_len = msg.length();
    user1.show_userinfo();
    std::string welcome_msg = reply.welcome_msg(user1);
    std::string whois_msg = reply.whois_msg(user1);
    // std::string yourhost_msg = reply.yourhost_msg(user1);
    // std::string created_msg = reply.created_msg(user1);
    // std::string myinfo_msg = reply.myinfo_msg(user1);
    // std::cout << " msg = " << welcome_msg << std::endl;
    send(socket, welcome_msg.c_str(), welcome_msg.size(), 0);
    // send(socket, whois_msg.c_str(), whois_msg.size(), 0);
    // std::cout << parts[0] << std::endl;
    // send(socket, "PONG", 4, 0);
    // send(socket, yourhost_msg.c_str(), yourhost_msg.size(), 0);
    // send(socket, created_msg.c_str(), created_msg.size(), 0);
    // send(socket, myinfo_msg.c_str(), myinfo_msg.size(), 0);
    }
    if (!cmd.empty() && parts[0] == "PING")
    {
    std::string pong_msg = reply.pong_msg(user1);
    std::cout << "pong [0] = |" << pong_msg << "|" << std::endl;
    send(socket, pong_msg.c_str(), pong_msg.size(), 0);
    }
}
