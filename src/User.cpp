/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaainouc <yaainouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 16:21:44 by yaainouc          #+#    #+#             */
/*   Updated: 2024/03/18 17:46:12 by agengemb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/User.hpp"
User::User(void) : nickname(""), username( ""),
	hostname(""), servername(""), realname(""), identifier("")
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
	std::cout << "Destruction du User : " << this->nickname << std::endl;
}

int User::get_isRegistered(void) const
{
	return(this->isRegistered);
}

void User::set_isRegistered(int i)
{
	this->isRegistered = i;
}

std::string const & User::get_hostname(void) const
{
	return(this->hostname);
}

std::string const & User::get_nickname(void) const
{
	return(this->nickname);
}

std::string const & User::get_realname(void) const
{
	return(this->realname);
}

std::string const & User::get_username(void) const
{
	return(this->username);
}

std::string const & User::get_servername(void) const
{
	return(this->servername);
}

std::string const & User::get_identifier(void) const
{
	return(this->identifier);
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
	this->identifier = this->nickname + "!" + this->username + "@" + this->hostname;
}
