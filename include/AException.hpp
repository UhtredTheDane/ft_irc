/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AException.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agengemb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:10:17 by agengemb          #+#    #+#             */
/*   Updated: 2024/04/27 19:17:30 by agengemb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AEXCEPTION_HPP
# define AEXCEPTION_HPP

# include <exception>
# include <string>
# include "User.hpp"
# include "Server_msg.hpp"

class Server_msg;

class AException : std::exception
{
	public:
        int get_socket(void);
        std::string& get_channel(void);
        std::string& get_nick(void);
        std::string& get_cmd(void);
    	virtual void handle(User* user, Server_msg* msg) = 0;
	    virtual ~AException(void) throw(){};
    protected:
        int socket;
        std::string channel;
        std::string nick;
        std::string cmd;
};
#endif
