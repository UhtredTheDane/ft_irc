/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaainouc <yaainouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:31:03 by agengemb          #+#    #+#             */
/*   Updated: 2024/04/28 17:05:23 by yaainouc         ###   ########.fr       */
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
	set_signal();
	try
	{
	Server serv(atoi(argv[1]), argv[2]);
	serv.run_server();
	}
	catch(std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	return (0);
}
