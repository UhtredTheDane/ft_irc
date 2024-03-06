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
#include <signal.h>

bool close_serv = false;

void sigint_handler(int signal)
{
	if (signal == SIGINT)
	{
		close_serv = true;
	}
}

void set_signal(void)
{
	struct sigaction action;
	bzero(&action, sizeof(action));
	action.sa_handler = &sigint_handler;
	sigaction(SIGINT, &action, NULL);
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Usage: ./ircserv port password" << std::endl;
		return (1);
	}
	set_signal();
	Server serv(atoi(argv[1]), argv[2]);
	serv.run_server();
	return (0);
}
