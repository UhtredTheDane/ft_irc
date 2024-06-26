/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaainouc <yaainouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 16:21:47 by yaainouc          #+#    #+#             */
/*   Updated: 2024/04/30 14:55:32 by yaainouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef USER_H
#define USER_H

#include <string>
#include <cstring>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>

class User
{
private:
	std::string nickname;
	std::string username;
	std::string hostname;
	std::string servername;
	std::string realname;
	std::string identifier;
	bool isPasswordValid;
	bool correctnick;
	int isRegistered;
	int socket;

public:
	User(int fd);
	// buffer a mettre en prive
	std::string buffer;
	~User();
	int get_socket(void) const;

	std::string const &get_nickname(void) const;
	std::string const &get_username(void) const;
	std::string const &get_realname(void) const;
	std::string const &get_hostname(void) const;
	std::string const &get_servername(void) const;
	std::string const &get_identifier(void) const;
	int get_isRegistered(void) const;
	bool get_isPasswordValid(void) const;
	void set_isPasswordValid(bool value);

	void set_correctnick(bool i);
	bool const & get_correctnick(void) const;
	void show_userinfo();
	void reset_userinfo();
	void set_socket(int socket);
	void set_nickname(std::string nickname);
	void set_username(std::string username);
	void set_realname(std::string realname);
	void set_hostname(std::string hostname);
	void set_servername(std::string hostname);
	void set_identifier(void);
	void set_isRegistered(int i);
};

#endif
