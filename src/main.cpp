/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agengemb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:31:03 by agengemb          #+#    #+#             */
/*   Updated: 2024/02/16 02:57:41 by agengemb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.hpp"
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

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Usage: ./ircserv port password" << std::endl;
		return (1);
	}


//Configuration du socket du serveur
	
	int fd = socket(AF_INET, SOCK_STREAM, 0); // sys/socket.h

	//rendre la socket serveur non bloquante
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
	struct sockaddr_in serv_addr; //netinet/in.h
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	int port_num = atoi(argv[1]);
	serv_addr.sin_port = htons(port_num);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		return (2);
	listen(fd, 5);




	std::vector<pollfd> *poll_fds = new std::vector<struct pollfd>(6);
	for (std::vector<pollfd>::iterator it = poll_fds->begin(); it != poll_fds->end(); ++it)
	{
		bzero(&(*it), sizeof(pollfd));
	}
	poll_fds->at(0).fd = fd;
	poll_fds->at(0).events = POLLIN;

	int num_connexion = 0;
	int addrlen = sizeof(serv_addr);
	while (1)
	{
		std::vector<pollfd> &test = *poll_fds;
		int request_nb;
		//std::cout << "AVANT POLL" << std::endl;
		request_nb = poll(test.data(), num_connexion + 1, -1);
		//std::cout << "APRES POLL" << std::endl;
		if (request_nb >= 1)
		{
			int fd_client;
			if ((fd_client = accept(fd, (struct sockaddr *) &serv_addr, (socklen_t*)&addrlen)) != -1)
			{
				fcntl(fd_client, F_SETFL, fcntl(fd_client, F_GETFL) | O_NONBLOCK);
				std::cout << "Une connexion" << std::endl;
				
				for (std::vector<struct pollfd>::iterator it = poll_fds->begin(); it != poll_fds->end() ; ++it)
				{
					if(it->fd == 0)
					{
						std::cout << "adding new client " << std::endl;
						it->fd = fd_client;
						it->events = POLLIN | POLLOUT;
						break;
					}
				}
				++num_connexion;
			}
			else
			{
				//std::cout << "error accept" << std::endl;
			}

			char buffer[1024];
			int bytes_nb;
			for (std::vector<struct pollfd>::iterator it = poll_fds->begin(); it != poll_fds->end(); ++it)
			{
				
				//std::cout << "ON RENTRE DANS LA BOUCLE ITERATOR" << std::endl;
				if (it->fd != 0 && it->revents & POLLIN)
				{
					bzero(buffer,1024);
					bytes_nb = recv(it->fd, buffer, 1024, 0);

					
					if(bytes_nb == 0)
					{
						std::cout << "a user leaved the server" << std::endl;
						close(it->fd);
						it->fd = 0;
						--num_connexion;
					}
					else
					{
						while (bytes_nb > 0)
						{
							if (bytes_nb == -1)
							{
								std::cout << "error\n";
								break;
							}
							else
							{
								std::string str_buffer(buffer);
								std::cout << "buffer: " << str_buffer << std::endl;
								create_user(it->fd, str_buffer);
							}
							bytes_nb = recv(it->fd, buffer, 1024, 0);
						}
					}
					/*
					for (std::vector<struct pollfd>::iterator it = poll_fds->begin(); it != poll_fds->end() ; ++it)
					{
						std::cout << it->fd << std::endl;
					}*/

					//std::cout << "APRES WHILE" << std::endl;
				}
				
			}
		}
	}
	return (0);
}
