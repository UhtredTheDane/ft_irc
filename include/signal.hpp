/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agengemb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 16:42:25 by agengemb          #+#    #+#             */
/*   Updated: 2024/04/26 16:42:26 by agengemb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_HPP
# define SIGNAL_HPP

# include <signal.h>
# include <strings.h>

void sigint_handler(int signal);
void set_signal(void);

#endif
