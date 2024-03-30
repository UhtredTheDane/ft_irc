/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaainouc <yaainouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 16:19:50 by agengemb          #+#    #+#             */
/*   Updated: 2024/03/26 17:26:48 by agengemb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "User.hpp"
# include "Server_msg.hpp"
# include <vector>
# include <strings.h>
# include <sys/types.h>
# include <iostream>
# include <cstdlib>
# include <sys/socket.h>
# include <netinet/in.h>
# include <poll.h>
# include <unistd.h>
# include <fcntl.h>
# include <map>
# include "Channel.hpp"

class User;

class Server
{
	public:
		Server_msg msg;
		Server(int port, std::string password);
		~Server(void);
		void run_server();
		void check_connection();
		void check_incoming_package();
		void request_handler(int client_socket, std::string &request);
		bool is_on_serv(std::string& nickname);
		
		void processing_request(int client_socket, User* user, std::string& request);
			void part_request(User* user);
			void privmsg_request(User* user);
			void kick_request(User* user);
			void mode_request(User* user);
			void join_request(User* user);
			void pong_request(User* user);

	private:
		int fd_socket;
		std::string password;
		int port;
		struct sockaddr_in serv_addr;
		std::vector<pollfd> *poll_fds;
		std::map<int, User*> users_map;
		std::string request_types[6];
		void (Server::*requests_ptr[6])(User*);
		std::map<std::string, Channel*> channels;
		std::vector<std::string> split_line;
};

#endif
