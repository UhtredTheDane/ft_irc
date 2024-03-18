/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaainouc <yaainouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 16:19:58 by agengemb          #+#    #+#             */
/*   Updated: 2024/03/17 15:53:35 by yaainouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"
#include <signal.h>
#include <string>
#include <sstream>

extern bool close_serv;

Server::Server(int port, std::string password)
{
	this->user = User();
    this->port = port;
    this->password = password;
	this->num_connexion = 0;
    //Configuration du socket du serveur
	this->fd_socket = socket(AF_INET, SOCK_STREAM, 0); // sys/socket.h
	//rendre la socket serveur non bloquante
	fcntl(this->fd_socket, F_SETFL, fcntl(this->fd_socket, F_GETFL) | O_NONBLOCK);
	//struct sockaddr_in serv_addr; //netinet/in.h
	bzero(&(this->serv_addr), sizeof(this->serv_addr));
	this->serv_addr.sin_family = AF_INET;
	this->serv_addr.sin_port = htons(port);
	this->serv_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(this->fd_socket, (struct sockaddr *)&(this->serv_addr), sizeof(this->serv_addr)) < 0)
		std::cout << "lancer une erreur" << std::endl;
	listen(this->fd_socket, 5);

	
	this->poll_fds = new std::vector<struct pollfd>(10);
	this->user_list = new std::list<User>();
	for (std::vector<pollfd>::iterator it = this->poll_fds->begin(); it != this->poll_fds->end(); ++it)
	{
		bzero(&(*it), sizeof(pollfd));
	}
	poll_fds->at(0).fd = this->fd_socket;
	poll_fds->at(0).events = POLLIN;
}

Server::~Server(void)
{
	for (std::vector<struct pollfd>::iterator it = poll_fds->begin(); it != poll_fds->end(); ++it)
	{
		close(it->fd);
	}
	delete poll_fds;
	close(fd_socket);
}

void Server::check_connection()
{
	int fd_client;
	int addrlen = sizeof(this->serv_addr);
	if ((fd_client = accept(this->fd_socket, (struct sockaddr *) &(this->serv_addr), (socklen_t*)&addrlen)) != -1)
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
}

void Server::reply(int socket)
{
    std::string msg = user.get_servername() + " ";
    msg += "003 ";
    msg += user.get_nickname();
    msg += " :Welcome to our irc server ";
    msg += user.get_identifier();
    msg += "\r\n";
    int msg_len = msg.length();
    send(socket, msg.c_str(), msg_len, 0);
}

void Server::connexion(int fd, std::string& request)
{
	std::stringstream coco(request);
	std::vector<std::string> split_line;
	std::string word;
	while (getline(coco, word, ' '))
		split_line.push_back(word);
	if (!split_line[0].compare("PASS"))
	{
		std::cout << "PASS pas fait\n";
	}
	else if (!split_line[0].compare("NICK"))
		user.set_nickname(split_line[1]);
	else if (!split_line[0].compare("USER"))
	{
		user.set_username(split_line[1]);
		user.set_hostname(split_line[2]);
		user.set_servername(split_line[3]);
		user.set_realname(split_line[3]);
		reply(fd);
	}
	else
		std::cout << "error connection request\n";
}

void Server::request_handler(int fd, std::string& request)
{
	User *tmp = 0;
	if (request.compare("CAP LS\r\n") == 0)
	{
		user.set_socket(fd);
	}
	else
	{
	//this->connexion(fd, request);
	if(tmp)
	{
		// std::cout << "oui baguette" << std::endl;
		//tmp->show_userinfo();
		user_list->push_back(*tmp);
		this->connexion(fd, request);
	}
	}
}

void Server::check_incoming_package()
{
	char buffer[1024];
	int bytes_nb;

	for (std::vector<struct pollfd>::iterator it = poll_fds->begin(); it != poll_fds->end(); ++it)
	{
		if (it->fd != 0 && it->revents & POLLIN)
		{
			while ((bytes_nb = recv(it->fd, buffer, 1024, 0)) != -1)
			{	
				if(bytes_nb == 0)
				{
					std::cout << "a user leaved the server" << std::endl;
					close(it->fd);
					it->fd = 0;
					user_list->erase(findUser(it->fd));
					--num_connexion;
					break;
				}
				std::string str_buffer(buffer);
				this->request_handler(it->fd, str_buffer);
				bzero(buffer, 1024);
			}
		}			
	}
	for (std::list<User>::iterator it = user_list->begin(); it != user_list->end(); ++it)
	{
		std::cout << it->get_socket() << std::endl ;
	}
}

void Server::run_server()
{
	while (!close_serv)
	{
		std::vector<pollfd> &test = *poll_fds;
		int request_nb;
		request_nb = poll(test.data(), this->num_connexion + 1, -1);
		if (request_nb >= 1)
		{
			check_connection();
			check_incoming_package();
		}
	}
}

std::list<User>::iterator Server::findUser(int fd)
{
	for (std::list<User>::iterator it = user_list->begin(); it != user_list->end(); ++it)
	{
		if(it->get_socket() == fd)
			return it;
	}
	return (user_list->end());
}