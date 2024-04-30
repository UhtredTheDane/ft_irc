/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agengemb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 17:39:52 by agengemb          #+#    #+#             */
/*   Updated: 2024/04/30 13:16:52 by agengemb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>
# include "User.hpp"

class Message
{
	private:
		std::string msg;
		User *user;
	public:
		std::string get_msg(void);
		Message(std::string& msg, User *user);
};
#endif
