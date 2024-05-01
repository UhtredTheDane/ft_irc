/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaainouc <yaainouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 16:21:44 by yaainouc          #+#    #+#             */
/*   Updated: 2024/04/30 14:53:37 by yaainouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/User.hpp"

User::User(int fd) : nickname(""), username( ""),
	hostname(""), servername(""), realname(""), identifier("")
{
	buffer = "";
	socket = fd;
	isRegistered = 0;
	correctnick = 0;
	isPasswordValid = false;
}

void User::show_userinfo()
{
	std::cout << "nickname : " << nickname << std::endl;
	std::cout << "username : " << username << std::endl;
	std::cout << "hostname : " << hostname << std::endl;
	std::cout << "servername : " << servername << std::endl;
	std::cout << "realname : " << realname << std::endl;
}

void User::reset_userinfo()
{
	set_nickname(""); 
	set_username(""); 
	set_hostname(""); 
	set_servername(""); 
	set_realname(""); 
	set_isPasswordValid(false);
}

User::~User()
{

}

int User::get_socket(void) const
{
	return (socket);
}

int User::get_isRegistered(void) const
{
	return(isRegistered);
}

bool User::get_isPasswordValid(void) const
{
	return (isPasswordValid);
}

void User::set_isPasswordValid(bool value)
{
	isPasswordValid = value;
}

void User::set_isRegistered(int i)
{
	isRegistered = i;
}

std::string const & User::get_hostname(void) const
{
	return(hostname);
}

void User::set_correctnick(bool i)
{
	correctnick = i;
}

bool const & User::get_correctnick(void) const
{
	return(correctnick);
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

void User::set_socket(int socket)
{
	this->socket = socket;
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
