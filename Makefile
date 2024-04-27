# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agengemb <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/03 18:01:24 by agengemb          #+#    #+#              #
#    Updated: 2024/04/26 16:40:19 by agengemb         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = c++
CFLAGS = -Wall -Wextra -std=c++98 -MMD
NAME = ircserv
SRC= src/main.cpp src/signal.cpp src/User.cpp src/Create_user.cpp src/Server.cpp src/Server_handler.cpp src/Server_msg.cpp src/Channel.cpp src/Message.cpp
OBJ= $(SRC:.cpp=.o)
DEP= ${OBJ:.o=.d}

all: $(NAME)

-include $(DEP)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJ) $(DEP)

fclean: clean
	rm -r $(NAME)

re: fclean all

.PHONY: all clean fclean re
