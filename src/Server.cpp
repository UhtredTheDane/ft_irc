/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaainouc <yaainouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 16:19:58 by agengemb          #+#    #+#             */
/*   Updated: 2024/03/17 17:28:23 by yaainouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"
#include <signal.h>
#include <string>
#include <sstream>

extern bool close_serv;

Server::Server(int port, std::string password)
{
	this->msg = Server_msg();
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
	(void) socket;
	this->msg.welcome_msg(user);
	this->msg.yourhost_msg(user);
	this->msg.created_msg(user);
	this->msg.myinfo_msg(user);
	this->msg.whois_msg(user);
}

void Server::connexion(int fd, std::string& request)
{
	// std::cout << "|" << request << "|" << std::endl;
	std::stringstream coco(request);
	std::vector<std::string> split_line;
	std::string word;
	while (getline(coco, word, ' '))
		split_line.push_back(word);
	if (user.get_isRegistered() == 0 && !request.compare("CAP LS"))
	{
		std::cout << "test\n";
		this->user.set_isRegistered(1);
	}
	else if(user.get_isRegistered() == 1)
	{
	if (!split_line[0].compare("PASS"))
	{
		std::cout << "PASS valide\n";
	}
	else if (!split_line[0].compare("NICK"))
		user.set_nickname(split_line[1]);
	else if (!split_line[0].compare("USER") && !user.get_nickname().empty())
	{
		user.set_username(split_line[1]);
		user.set_hostname(split_line[2]);
		user.set_servername(split_line[3]);
		user.set_realname(split_line[3]);
		user.set_socket(fd);
		reply(fd);
		this->user.set_isRegistered(2);
	
	}
	}
	else
		 std::cout << "|" << request << "|" << std::endl;

}

void Server::request_handler(int fd, std::string& request)
{
	(void) fd;
	std::string buf = "\r\n";
	if (request.find(buf, 0) != std::string::npos)
	{
		this->user.buffer += request;
		std::string delimiter = "\r\n";
		std::string token;
		// std::vector<std::string> split_line;
		while(this->user.buffer.find(delimiter) != std::string::npos)
		{
			token = this->user.buffer.substr(0, this->user.buffer.find(delimiter));
			this->user.buffer.erase(0, this->user.buffer.find(delimiter) + delimiter.length());
			connexion(fd, token);
		}
		// //traitement
	}
	else
	{
		this->user.buffer += request;
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
			bzero(buffer, 1024);
			while ((bytes_nb = recv(it->fd, buffer, 1024, 0)) != -1)
			{	
				if(bytes_nb == 0)
				{
					std::cout << "a user leaved the server" << std::endl;
					close(it->fd);
					it->fd = 0;
					--num_connexion;
					break;
				}
				std::string str_buffer(buffer);
				this->request_handler(it->fd, str_buffer);
				bzero(buffer, 1024);
			}
		}			
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
