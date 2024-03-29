/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaainouc <yaainouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 16:19:58 by agengemb          #+#    #+#             */
/*   Updated: 2024/03/30 00:29:36 by agengemb         ###   ########.fr       */
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
	msg = Server_msg();
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

void Server::reply(User *user)
{
	msg.welcome_msg(user);
	msg.yourhost_msg(user);
	msg.created_msg(user);
	msg.myinfo_msg(user);
	//msg.whois_msg(user, client_socket);
}

bool Server::is_on_serv(std::string& nickname)
{
	for (std::map<int, User*>::iterator it = users_map.begin(); it != users_map.end(); ++it)
	{
		if (it->second->get_nickname() == nickname)
			return (true);
	}
	return (false);
}

void Server::connexion(int client_socket, User* user, std::string& request)
{
	std::stringstream coco(request);
	std::vector<std::string> split_line;
	std::string word;

	while (getline(coco, word, ' '))
		split_line.push_back(word);
	if (user->get_isRegistered() == 0 && !request.compare("CAP LS"))
	{
		user->set_isRegistered(1);
	}
	else if(user->get_isRegistered() == 1)
	{
		if (!split_line[0].compare("PASS"))
		{
			std::cout << "PASS valide\n";
		}
		else if (!split_line[0].compare("NICK"))
		{
			while (is_on_serv(split_line[1]))
				split_line[1] += "_";
			user->set_nickname(split_line[1]);
		}
		else if (!split_line[0].compare("USER") && !user->get_nickname().empty())
		{
				user->set_username(split_line[1]);
				user->set_hostname(split_line[2]);
				user->set_servername(split_line[3]);
				user->set_realname(split_line[3]);
				user->set_socket(client_socket);
				user->set_identifier();
				// user.show_userinfo(user);
				reply(user);
				user->set_isRegistered(2);
		}
	}

	


	if (!split_line[0].compare("PING"))
	{
		msg.pong_msg(user);
	}
	else if (!split_line[0].compare("JOIN"))
	{
		try
		{	
			std::cout << user->get_username() << std::endl;
			Channel* current_chan = channels.at(split_line[1]);
			current_chan->add_user(user);
			msg.join_msg(user, current_chan);

			std::string join_msg;
			join_msg += ":" + user->get_nickname() + "!" + user->get_nickname() + "@localhost JOIN :" + current_chan->get_name();
			join_msg += "\r\n";
			for (std::vector<User*>::iterator it = current_chan->get_users()->begin(); it != current_chan->get_users()->end(); ++it)
			{
				if (user != *it)
					send((*it)->get_socket(), join_msg.c_str(), join_msg.length(), 0);
			}

		}
		catch (std::out_of_range& oor)
		{
			Channel* new_chan = new Channel(split_line[1], user);
			channels.insert(std::pair<std::string, Channel*>(split_line[1], new_chan));
			msg.join_msg(user, new_chan);
		}
	}
	else if (!split_line[0].compare("MODE") && split_line[1][0] == '#')
	{
		try
		{
			Channel* target_chan = channels.at(split_line[1]);
			msg.mode_msg(user, target_chan);
		}
		catch (std::out_of_range& oor)
		{
		}
	}
	else if (!split_line[0].compare("KICK"))
	{
		std::cout << request << std::endl;
		Channel *curent_chan = channels.at(split_line[1]);
		for (std::vector<User*>::iterator it = curent_chan->get_admins()->begin(); it != curent_chan->get_admins()->end();)
		{
			if(user == *it)
			{
				std::string k_msg;
				for (std::vector<User*>::iterator it2 = curent_chan->get_users()->begin(); it2 != curent_chan->get_users()->end();)
					{
						if((*it2)->get_nickname() == split_line[2])
							{
								k_msg = ":" + user->get_identifier() + " KICK " + curent_chan->get_name() + " " + split_line[2] + " " + split_line[3] + "\r\n";
								send((*it2)->get_socket(), k_msg.c_str(), k_msg.length(), 0);
								break;
							}
						it2++;
					}
			}
			it++;
		}
	}

	else if (!split_line[0].compare("PRIVMSG"))
	{

		try
		{
			if(split_line[1][0] == '#')
			{
			Channel *curent_chan = channels.at(split_line[1]); 
			Message msg(split_line[2], user);
			curent_chan->add_message(&msg);
			std::string c_msg;
			for (std::vector<User*>::iterator it = curent_chan->get_users()->begin(); it != curent_chan->get_users()->end();)
			{
				if(*it != user)
				{
				c_msg = ":" + user->get_identifier() + " PRIVMSG " + curent_chan->get_name()+ " " + msg.get_msg() + "\r\n";
				std::cout << c_msg << std::endl;
				send((*it)->get_socket(), c_msg.c_str(), c_msg.length(), 0);
				}
			it++;
			}
			}
			else
			{

			for (std::map<int, User*>::iterator it = users_map.begin(); it != users_map.end(); ++it)
			{
				if (it->second->get_nickname() == split_line[1])
				{
					std::string p_msg;
					p_msg = ":" + user->get_nickname() + " PRIVMSG " + it->second->get_nickname()+ " " + split_line[2] + "\r\n";
					std::cout << p_msg << std::endl;
					send(it->first, p_msg.c_str(), p_msg.length(), 0);
					break;
				}
			}
			}
		}
		catch (std::out_of_range& oor)
		{
		}
	}
	else if (!split_line[0].compare("PART"))
	{
		
		std::cout << "|" << request << "|" << std::endl;
		try
		{
			Channel *current_chan = channels.at(split_line[1]); 
			msg.leave_msg(user, current_chan);
			current_chan->delete_user(user);
		}
		catch (std::out_of_range& oor)
		{
		}
	}

	else
		std::cout << "|" << request << "|" << std::endl;
}

void Server::request_handler(int client_socket, std::string& request)
{
	User* user;
	std::string delimiter = "\r\n";
	std::string token;
	size_t delim_pos;

	user = users_map.at(client_socket);
	if (request.find(delimiter, 0) != std::string::npos)
	{
		user->buffer += request;
		while((delim_pos = user->buffer.find(delimiter)) != std::string::npos)
		{
			token = user->buffer.substr(0, delim_pos);
			connexion(client_socket, user, token);
			user->buffer.erase(0, delim_pos + delimiter.length());
		}
	}
	else
	{
		user->buffer += request;
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
					request_handler(it->fd, str_buffer);
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
