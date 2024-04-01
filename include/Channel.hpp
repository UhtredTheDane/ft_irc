/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agengemb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:17:28 by agengemb          #+#    #+#             */
/*   Updated: 2024/03/21 08:39:23 by agengemb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <vector>
# include "User.hpp"
# include "Message.hpp"


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
		std::vector<Message*> msgs;
		std::string name;
	public:
		Channel(void);
		Channel(std::string& theme, User* admin_user);
		~Channel(void);
		void add_user(User *user);
		void add_message(Message *message);
		std::string get_theme( void );
		std::vector<User*>* get_users(void);
		void update_mod(User *user, std::vector<std::string> line);
		void give_privilege(User *user);
		void take_privilege(User *user);
		void remove_mod(User *user, int modif);
		void set_mod(User *user, int modif );
		std::string getName();
};
#endif
