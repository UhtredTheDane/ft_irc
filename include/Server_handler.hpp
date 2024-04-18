/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_handler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agengemb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 16:07:32 by agengemb          #+#    #+#             */
/*   Updated: 2024/04/10 23:52:00 by agengemb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HANDLER_HPP
# define SERVER_HANDLER_HPP

# include <exception>
# include <vector>
# include <string>
# include "User.hpp"
# include "Channel.hpp"
# include "Server_msg.hpp"
# include "Server.hpp"


class Server;

class Server_handler
{

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

		class Err_PasswordIncorrect : public std::exception{};
		class Err_AlreadyRegistred : public std::exception{};
		class Err_NoSuchChannel : public std::exception
		{
			public:
				Err_NoSuchChannel(std::string str);
				std::string get_str(void);
				virtual ~Err_NoSuchChannel(void) throw(){};
			private:
				std::string str;
		};
		class Err_NeedMoreParams : public std::exception{};
		class Err_NoSuchNick : public std::exception
		{
			public:
				Err_NoSuchNick(std::string str);
				std::string get_str(void);
				virtual ~Err_NoSuchNick(void) throw(){};
			private:
				std::string str;
		};
		class Err_CannotSendToChan: public std::exception
		{
				public:
				Err_CannotSendToChan(std::string str);
				std::string get_str(void);
				virtual ~Err_CannotSendToChan(void) throw(){};
			private:
				std::string str;
		};
		class Err_NotOnChannel : public std::exception
		{
			public:
				Err_NotOnChannel(std::string str);
				std::string get_str(void);
				virtual ~Err_NotOnChannel(void) throw(){};
			private:
				std::string str;
		};


	private:
		Server* serv;
		Server_msg msg;
		std::string raw_msg;
		std::vector<std::string> split_line;
		std::string request_types[11];
		void (Server_handler::*requests_ptr[11])(User*);
};

#endif
