/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Create_user.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaainouc <yaainouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 16:21:35 by yaainouc          #+#    #+#             */
/*   Updated: 2024/03/17 15:51:06 by yaainouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/User.hpp"
#include "stdio.h"
std::vector<std::string> splitString(const char in[])
{
    std::istringstream iss(in);
    std::istream_iterator<std::string> first(iss), last;

    std::vector<std::string> parts;
    std::copy(first, last, std::back_inserter(parts));
    return parts;
}
