/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaainouc <yaainouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 16:21:44 by yaainouc          #+#    #+#             */
/*   Updated: 2024/03/18 19:06:14 by agengemb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/User.hpp"
User::User(void) : nickname(""), username( ""),
	hostname(""), servername(""), realname(""), identifier("")
{
	buffer = "";
	// update_identifier();
}

void User::show_userinfo()
{
	std::cout << "nickname : " << nickname << std::endl;
	std::cout << "username : " << username << std::endl;
	std::cout << "hostname : " << hostname << std::endl;
	std::cout << "servername : " << servername << std::endl;
	std::cout << "realname : " << realname << std::endl;
}
User::~User()
{
	std::cout << "Destruction du User : " << nickname << std::endl;
}

int User::get_isRegistered(void) const
{
	return(isRegistered);
}

void User::set_isRegistered(int i)
{
	isRegistered = i;
}

std::string const & User::get_hostname(void) const
{
	return(hostname);
}

std::string const & User::get_nickname(void) const
{
	return(nickname);
}

std::string const & User::get_realname(void) const
{
	return(realname);
}

std::string const & User::get_username(void) const
{
	return(username);
}

std::string const & User::get_servername(void) const
{
	return(servername);
}

std::string const & User::get_identifier(void) const
{
	return(identifier);
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
void User::set_identifier(void)
{
	this->identifier = nickname + "!" + username + "@" + hostname;
}
