/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaainouc <yaainouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 16:19:50 by agengemb          #+#    #+#             */
/*   Updated: 2024/03/15 03:15:16 by agengemb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "User.hpp"
#include <vector>
#include <strings.h>
#include <sys/types.h>
#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>


class Server
{
  public:
    Server(int port, std::string password);
    ~Server(void);
    void run_server();
    void check_connection();
    void check_incoming_package();
	void test(int fd, std::string &buffer);
	void connexion(int fd);
  private:
    User user;
    int fd_socket;
    std::string password;
    int port;
    int num_connexion;
    struct sockaddr_in serv_addr;
    std::vector<pollfd> *poll_fds;
};

void create_user(int socket, std::string cmd, std::vector<pollfd> *poll_fds);

#endif
