/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaainouc <yaainouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 16:19:50 by agengemb          #+#    #+#             */
/*   Updated: 2024/04/30 13:16:57 by agengemb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

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
# include "Server_handler.hpp"
# include <map>
#include "signal.hpp"

class Server_handler;

class Server
{
	public:

		Server(int port, std::string password);
		~Server(void);
		bool check_password(std::string password);
		void add_user(int fd_client);
		void delete_user(int fd_client);
		Channel* add_channel(std::string name, User* user);
		std::map<std::string, Channel*> get_channels(void);
		std::map<int, User*> get_users(void);
		bool is_on_serv(std::string& nickname);
		void run_server();
		void check_connection();
		void check_incoming_package();
		User *findUserByName(std::string name);

	private:

		Server_handler* handler;
		int fd_socket;
		std::string password;
		int port;
		struct sockaddr_in serv_addr;
		std::vector<pollfd> *poll_fds;
		std::map<int, User*> users_map;
		std::map<std::string, Channel*> channels;
};

#endif
