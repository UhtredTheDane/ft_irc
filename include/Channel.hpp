/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agengemb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:17:28 by agengemb          #+#    #+#             */
/*   Updated: 2024/03/28 01:30:19 by agengemb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <vector>
# include "User.hpp"
# include "Message.hpp"
#include <sys/types.h>
#include <sys/socket.h>

class Channel
{
	enum Chanmod { i = 1 , k = 2 , o = 3 , l = 4 , t = 5};

	private:
		//mod a rajouter
		std::string theme;
		std::string password;
		int limit_user;
		int mask;
		std::vector<User*> admin_users;
		std::vector<User*> users;
		std::vector<User*> invite;
		std::vector<Message*> msgs;
		std::string name;
	public:
		Channel(void);
		Channel(std::string& theme, User* admin_user);
		~Channel(void);
		void add_user(User *user);
		void delete_user(User* to_delete);
		void add_message(Message *message);
		std::string get_theme( void );
		std::string get_name( void );
		std::vector<User*>* get_users(void);
		std::vector<User*>* get_admins(void);
		void invite_user(User *user);
		void erase_invite(User* user);
		void update_mod(int clientsocket ,User *user, std::vector<std::string> line);
		int give_privilege(User *user,std::string name);
		int take_privilege(User *user,std::string name);
		int remove_mod(User *user, int modif);
		int set_mod(User *user, int modif );
		User *findUserByName(std::vector<User *> v,std::string name);
		int IsMod(User *user);
		int IsOption(int option);
};
#endif
