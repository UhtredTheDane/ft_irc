/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaainouc <yaainouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 16:19:50 by agengemb          #+#    #+#             */
/*   Updated: 2024/04/03 15:30:10 by agengemb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Server_handler.hpp"
# include <vector>
# include <strings.h>
# include <sys/types.h>
# include <iostream>
# include <cstdlib>
# include <sys/socket.h>
# include <netinet/in.h>
# include <poll.h>
# include <unistd.h>
# include <fcntl.h>=

class Server_handler;

class Server
{
	public:
		Server(int port, std::string password);
		~Server(void);
		void run_server();
		void check_connection();
		void check_incoming_package();

	private:
		Server_handler handler;
		int fd_socket;
		std::string password;
		int port;
		struct sockaddr_in serv_addr;
		std::vector<pollfd> *poll_fds;
};

#endif
