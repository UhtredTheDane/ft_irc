/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaainouc <yaainouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 16:21:44 by yaainouc          #+#    #+#             */
/*   Updated: 2024/03/05 16:21:45 by yaainouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/User.hpp"

User::User(int socket) : socket(socket), nickname(""), username( ""),
	hostname(""), servername(""), realname("")
{
	// update_identifier();
}
void User::show_userinfo()
{
	std::cout << "nickname : " << this->nickname << std::endl;
	std::cout << "username : " << this->username << std::endl;
	std::cout << "hostname : " << this->hostname << std::endl;
	std::cout << "servername : " << this->servername << std::endl;
	std::cout << "realname : " << this->realname << std::endl;
}
User::~User()
{
	// close( this->socket );
}


void User::set_hostname(std::string hostname)
{
	this->hostname = hostname;
}

void User::set_nickname(std::string nickname)
{
	this->nickname = nickname;
}

void User::set_realname(std::string realname)
{
	this->realname = realname;
}

void User::set_username(std::string username)
{
	this->username = username;
}

void User::set_servername(std::string servername)
{
	this->servername = servername;
}