
Server_handler::Server_handler(Server* serv)
{
    msg = Server_msg();
    this->serv = serv;
    request_types[0] = "PING";
	request_types[1] = "JOIN";
	request_types[2] = "MODE";
	request_types[3] = "KICK"; 
	request_types[4] = "PRIVMSG";
	request_types[5] = "PART";
	requests_ptr[0] = &Server_handler::pong_request;
	requests_ptr[1] = &Server_handler::join_request;
	requests_ptr[2] = &Server_handler::mode_request;
	requests_ptr[3] = &Server_handler::kick_request;
	requests_ptr[4] = &Server_handler::privmsg_request;
	requests_ptr[5] = &Server_handler::part_request;
}

void Server_handler::pong_request(User* user)
{
	msg.pong_msg(user);
}

void Server_handler::join_request(User* user)
{
		try
		{	
			std::cout << user->get_username() << std::endl;
			Channel* current_chan = serv.get_channels().at(split_line[1]);
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
			msg.join_msg(user, serv->add_channel(split_line[1]));
		}
}

void Server_handler::mode_request(User* user)
{
		try
		{
			Channel* target_chan = serv->get_channels().at(split_line[1]);
			msg.mode_msg(user, target_chan);
		}
		catch (std::out_of_range& oor)
		{
		}
}

void Server_handler::kick_request(User* user)
{
		Channel *curent_chan = serv->get_channels().at(split_line[1]);
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

void Server_handler::privmsg_request(User* user)
{
	try
		{
			if(split_line[1][0] == '#')
			{
				Channel *curent_chan = serv->get_channels().at(split_line[1]); 
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
                std::map<int, User*> users_map = serv->get_users();
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

void Server_handler::part_request(User* user)
{
		try
		{
			Channel *current_chan = serv->get_channels().at(split_line[1]); 
			msg.leave_msg(user, current_chan);
			current_chan->delete_user(user);
		}
		catch (std::out_of_range& oor)
		{
		}
}

void Server_handler::processing_request(int client_socket, User* user, std::string& request)
{
	std::stringstream coco(request);
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
				msg.welcome_msg(user);
				msg.yourhost_msg(user);
				msg.created_msg(user);
				msg.myinfo_msg(user);
				user->set_isRegistered(2);
		}
	}

	for (int i = 0; i < 6; ++i)
	{
		if (!split_line[0].compare(request_types[i]))
		{
			(this->*requests_ptr[i])(user);
		}
	}
	split_line.clear();
}

bool Server_handler::is_on_serv(std::string& nickname)
{
    std::map<int, User*> users_map = serv->get_users();
	for (std::map<int, User*>::iterator it = users_map.begin(); it != users_map.end(); ++it)
	{
		if (it->second->get_nickname() == nickname)
			return (true);
	}
	return (false);
}


void Server_handler::request_handler(int client_socket, std::string& request)
{
	User* user;
	std::string delimiter = "\r\n";
	std::string token;
	size_t delim_pos;

    std::map<int, User*> users_map = serv->get_users();
	user = users_map.at(client_socket);
	if (request.find(delimiter, 0) != std::string::npos)
	{
		user->buffer += request;
		while((delim_pos = user->buffer.find(delimiter)) != std::string::npos)
		{
			token = user->buffer.substr(0, delim_pos);
			processing_request(client_socket, user, token);
			user->buffer.erase(0, delim_pos + delimiter.length());
		}
	}
	else
	{
		user->buffer += request;
	}
}
