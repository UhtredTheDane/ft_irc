/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaainouc <yaainouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 16:19:50 by agengemb          #+#    #+#             */
/*   Updated: 2024/03/18 17:52:20 by agengemb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "User.hpp"
#include "Server_msg.hpp"
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
#include <map>

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
	  void connexion(int client_socket, User* user, std::string& request);
	  void reply(int socket);
  private:
    int fd_socket;
    std::string password;
    int port;
    struct sockaddr_in serv_addr;
    std::vector<pollfd> *poll_fds;
    std::map<int, User*> users_map;
};

#endif
