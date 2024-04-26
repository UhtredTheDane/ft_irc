/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_handler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agengemb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 16:07:32 by agengemb          #+#    #+#             */
/*   Updated: 2024/04/19 22:16:00 by agengemb         ###   ########.fr       */
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
		class Err_InviteOnlyChan : public std::exception
		{
			public:
				Err_InviteOnlyChan(std::string channel);
				std::string get_channel(void);
				virtual ~Err_InviteOnlyChan(void) throw(){};
			private:
				std::string channel;	
		};
		class Err_ChannelIsFull : public std::exception
		{
			public:
				Err_ChannelIsFull(std::string channel);
				std::string get_channel(void);
				virtual ~Err_ChannelIsFull(void) throw(){};
			private:
				std::string channel;	
		};
		class Err_BadChannelKey : public std::exception
		{
			public:
				Err_BadChannelKey(std::string channel);
				std::string get_channel(void);
				virtual ~Err_BadChannelKey(void) throw(){};
			private:
				std::string channel;	
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
		class Err_useronchannel : public std::exception
		{
			public:
				Err_useronchannel(std::string nick, std::string channel);
				std::string getNick();
				std::string getChannel();
				virtual ~Err_useronchannel(void) throw(){};
			private :
				std::string channel;
				std::string nick;
		};
		class Err_UserNotInChannel : public std::exception
		{
			public:
				Err_UserNotInChannel(std::string nick, std::string channel);
				std::string getNick();
				std::string getChannel();
				virtual ~Err_UserNotInChannel(void) throw(){};
			private :
				std::string channel;
				std::string nick;
		};
		class Err_chanoprivsneeded : public std::exception
		{
			public:
				Err_chanoprivsneeded(std::string channel);
				std::string getNick();
				std::string getChannel();
				virtual ~Err_chanoprivsneeded(void) throw(){};
			private :
				std::string channel;
				std::string nick;
		};
		/*
		class Err_nosuchnick : public std::exception
		{
			public:
				Err_nosuchnick(std::string str);
				virtual ~Err_nosuchnick(void) throw(){};
		};
 	   
		*/
	
};

#endif
