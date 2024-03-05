/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaainouc <yaainouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 16:19:50 by agengemb          #+#    #+#             */
/*   Updated: 2024/03/05 17:44:43 by yaainouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
 #include <strings.h>
#include <sys/types.h>
#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>

class Server
{
  public:
    Server(int port, std::string password);
    void run_server();
    void check_connection();
    void check_incoming_package();

  private:
    int fd_socket;
    std::string password;
    int port;
    int num_connexion;
    struct sockaddr_in serv_addr;
    std::vector<pollfd> *poll_fds;
};

void create_user(int socket, std::string cmd);

#endif
