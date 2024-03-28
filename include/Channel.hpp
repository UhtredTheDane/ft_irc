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

class Channel
{
	private:
		//mod a rajouter
		std::string theme;
		std::string name;
		std::vector<User*> admin_users;
		std::vector<User*> users;
		std::vector<Message*> msgs;
		
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

};
#endif
