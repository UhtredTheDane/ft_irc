/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_handler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agengemb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 16:07:32 by agengemb          #+#    #+#             */
/*   Updated: 2024/04/27 19:21:15 by agengemb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HANDLER_HPP
# define SERVER_HANDLER_HPP

# include <vector>
# include <string>
# include "User.hpp"
# include "Channel.hpp"
# include "Server_msg.hpp"
# include "Server.hpp"
# include "AException.hpp"
# include "IrcExceptions.hpp"

class Server;

class Server_handler
{
	
	private:
			
			Server* serv;
			Server_msg msg;
			std::string raw_msg;
			std::vector<std::string> split_line;
			std::string request_types[11];
			void (Server_handler::*requests_ptr[11])(User*);
	
	public:

		Server_handler(Server* serv);
		void request_handler(int client_socket, std::string &request);
		void processing_request(User* user, std::string& request);
		void capls_request(User* user);
		void pass_request(User* user);
		void nick_request(User* user);
		void user_request(User* user);
		void part_request(User* user);
		void privmsg_request(User* user);
		void kick_request(User* user);
		void mode_request(User* user);
		void join_request(User* user);
		void pong_request(User* user);
		void invite_request(User* user);
		void msg_toall(std::vector<std::string> split_line, User* user, std::string t_request);
		User *findUserByName(std::vector<User *> v,std::string name);
		Server_msg* get_msg(void);
		
};
#endif
