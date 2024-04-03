/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaainouc <yaainouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 16:19:58 by agengemb          #+#    #+#             */
/*   Updated: 2024/04/03 15:39:48 by agengemb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"
#include <signal.h>
#include <string>
#include <sstream>
#include <stdexcept>

extern bool close_serv;

Server::Server(int port, std::string password)
{
	handler = Server_handler();
	this->port = port;
	this->password = password;

	//Configuration du socket du serveur
	fd_socket = socket(AF_INET, SOCK_STREAM, 0); // sys/socket.h

	//rendre la socket serveur non bloquante
	fcntl(fd_socket, F_SETFL, fcntl(fd_socket, F_GETFL) | O_NONBLOCK);

	//struct sockaddr_in serv_addr; //netinet/in.h
	bzero(&(serv_addr), sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(fd_socket, (struct sockaddr *)&(serv_addr), sizeof(serv_addr)) < 0)
		std::cout << "lancer une erreur" << std::endl;
	listen(fd_socket, 5);

	poll_fds = new std::vector<pollfd>(1);
	bzero(&poll_fds->at(0), sizeof(pollfd));
	poll_fds->at(0).fd = fd_socket;
	poll_fds->at(0).events = POLLIN;
}

Server::~Server(void)
{
	for (std::vector<pollfd>::iterator it = poll_fds->begin(); it != poll_fds->end(); ++it)
	{
		close(it->fd);
	}
	delete poll_fds;
	close(fd_socket);
}

std::map<std::string, Channel*> Server::get_channels(void)
{
	return (channels);
}

std::map<int, User*> Server::get_users(void)
{
	return (users_map);
}

Channel* Server::add_channel(std::string name, User* user)
{
	Channel* new_chan = new Channel(name, user);
	channels.insert(std::pair<std::string, Channel*>(name, new_chan));
	return (new_chan);
}

void Server::check_connection()
{
	int fd_client;
	int addrlen = sizeof(serv_addr);
	if ((fd_client = accept(fd_socket, (struct sockaddr *) &(serv_addr), (socklen_t*)&addrlen)) != -1)
	{
		fcntl(fd_client, F_SETFL, fcntl(fd_client, F_GETFL) | O_NONBLOCK);
		std::cout << "adding new client " << std::endl;
		pollfd new_pollfd;
		new_pollfd.events = POLLIN | POLLOUT;
		new_pollfd.fd = fd_client;	
		poll_fds->push_back(new_pollfd);
		users_map.insert(std::pair<int, User*>(fd_client, new User()));
	}
	else
	{
		//std::cout << "error accept" << std::endl;
	}
}

void Server::check_incoming_package()
{
	char buffer[1024];
	int bytes_nb;

	for (std::vector<struct pollfd>::iterator it = poll_fds->begin(); it != poll_fds->end();)
	{
		if (it->fd != fd_socket && it->revents & POLLIN)
		{
			while ((bytes_nb = recv(it->fd, buffer, 1024, 0)) != -1)
			{	
				if (bytes_nb == 0)
				{
					std::cout << "a user leaved the server" << std::endl;
					close(it->fd);
					users_map.erase(it->fd);
					it = poll_fds->erase(it);
					break;
				}
				else
				{
					std::string str_buffer(buffer);
					handler.request_handler(it->fd, str_buffer);
					bzero(buffer, 1024);
					++it;
				}
			}
		}
		else
			++it;		
	}
}

void Server::run_server()
{
	while (!close_serv)
	{
		std::vector<pollfd> &test = *poll_fds;
		int request_nb;
		request_nb = poll(test.data(), test.size(), -1);
		if (request_nb >= 1)
		{
			check_connection();
			check_incoming_package();
		}
	}
}
