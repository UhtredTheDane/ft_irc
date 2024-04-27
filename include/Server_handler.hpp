/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_handler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agengemb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 16:07:32 by agengemb          #+#    #+#             */
/*   Updated: 2024/04/27 17:07:34 by agengemb         ###   ########.fr       */
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
		
		class Err_PasswordIncorrect : public AException
		{
			public:
				void handle(User* user, Server_msg* msg)
				{
					msg->passwordincorrect_msg(user);
				};

		};
		class Err_AlreadyRegistred : public AException
		{
			public:
				void handle(User* user, Server_msg* msg)
				{
					msg->alreadyregistred_msg(user);
				};
		};
		class Err_NotRegistred: public AException
		{
			public:
				Err_NotRegistred(int socket);
				void handle(User* user, Server_msg* msg)
				{
					(void) user;
					msg->notregistred_msg(get_socket());
				};

		};
		class Err_NeedMoreParams : public AException
		{
			public:
				Err_NeedMoreParams(std::string channel);
				void handle(User* user, Server_msg* msg)
				{
					std::string strtest = get_channel();
					msg->needmoreparams_msg(user, strtest);
				};
		};
		class Err_InviteOnlyChan : public AException
		{
			public:
				Err_InviteOnlyChan(std::string channel);
				void handle(User* user, Server_msg* msg)
				{
					std::string strtest = get_channel();
					msg->inviteonlychan_msg(user, strtest);
				};	
		};
		class Err_ChannelIsFull : public AException
		{
			public:
				Err_ChannelIsFull(std::string channel);
				void handle(User* user, Server_msg* msg)
				{
					std::string strtest = get_channel();
					msg->channelisfull_msg(user, strtest);
				};	
		};
		class Err_NoSuchChannel : public AException
		{
			public:
				Err_NoSuchChannel(std::string str);
				void handle(User* user, Server_msg* msg)
				{
					std::string strtest = get_channel();
					msg->nosuchchannel_msg(user, strtest);
				};
		};
		class Err_BadChannelKey : public AException
		{
			public:
				Err_BadChannelKey(std::string channel);
				void handle(User* user, Server_msg* msg)
				{
					std::string strtest = get_channel();
					msg->badchannelkey_msg(user, strtest);
				};	
		};

		class Err_NoSuchNick : public AException
		{
			public:
				Err_NoSuchNick(std::string str);
				void handle(User* user, Server_msg* msg)
				{
					std::string strtest = get_channel();
					msg->nosuchnick_msg(user, strtest);;
				};
		};
		class Err_CannotSendToChan: public AException
		{
			public:
				Err_CannotSendToChan(std::string str);
				void handle(User* user, Server_msg* msg)
				{
					std::string strtest = get_channel();
					msg->cannotsendtochan_msg(user, strtest);;
				};
		};
		class Err_NotOnChannel : public AException
		{
			public:
				Err_NotOnChannel(std::string str);
				void handle(User* user, Server_msg* msg)
				{
					std::string strtest = get_channel();
					msg->notonchannel_msg(user, strtest);
				};
		};
		class Err_chanoprivsneeded : public AException
		{
			public:
				Err_chanoprivsneeded(std::string channel);
				void handle(User* user, Server_msg* msg)
				{
					std::string strtest = get_channel();
					msg->err_chanoprivneeded_msg(user, strtest);
				};
		};
		class Err_useronchannel : public AException
		{
			public:
				Err_useronchannel(std::string nick, std::string channel);
				void handle(User* user, Server_msg* msg)
				{
					msg->err_useronchannel_msg(user, get_channel(), get_nick());
				};

		};
		class Err_UserNotInChannel : public AException
		{
			public:
				Err_UserNotInChannel(std::string nick, std::string channel);
				void handle(User* user, Server_msg* msg)
				{
					msg->err_useronchannel_msg(user, get_channel(), get_nick());
				};
		};
};
#endif
