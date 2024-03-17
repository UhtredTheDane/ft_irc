/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaainouc <yaainouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 16:19:50 by agengemb          #+#    #+#             */
/*   Updated: 2024/03/17 15:57:41 by yaainouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <list>
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

class User;
class Server
{
  public:
    Server(int port, std::string password);
    ~Server(void);
    void run_server();
    void check_connection();
    //void check_incoming_package();
	  //void request_handler(int fd, std::string &request);
	  void connexion(int fd, std::string& request);
	  void reply(int socket);

  private:
    int fd_socket;
    std::string password;
    int port;
    int num_connexion;
    struct sockaddr_in serv_addr;
    std::list<User*> user_list;
   // std::list<User>::iterator findUser(int fd);
};

User *create_user(int socket, std::string cmd);

#endif
