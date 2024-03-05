/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaainouc <yaainouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 16:21:47 by yaainouc          #+#    #+#             */
/*   Updated: 2024/03/05 16:21:48 by yaainouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_H
#define USER_H

# include <string>
# include <cstring>
# include <exception>
# include <stdexcept>
# include <iostream>
# include <vector>
# include <sstream>
# include <iterator>
# include <algorithm>


class User
{
    private:
        int socket;
        std::string nickname;
		std::string username;
		std::string hostname;
        std::string servername;
		std::string realname;
		bool isPasswordValid;
    
	public:

        User(int socket);
    	virtual ~User();
    	std::string const & get_nickname( void ) const;
		std::string const & get_username( void ) const;
		std::string const & get_realname( void ) const;
		std::string const & get_hostname( void ) const;
        std::string const & get_servername( void ) const;
		
		void show_userinfo();
    	void set_nickname( std::string nickname );
    	void set_username( std::string username );
		void set_realname( std::string realname );
		void set_hostname( std::string hostname );
		void set_servername( std::string hostname );
};

#endif