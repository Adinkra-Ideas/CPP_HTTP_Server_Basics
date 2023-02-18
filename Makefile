# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: euyi <euyi@student.42wolfsburg.de>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/01 16:57:06 by euyi              #+#    #+#              #
#    Updated: 2023/02/18 19:14:59 by euyi             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	http_serv

SRC		=	main.cpp	TcpServer.cpp

OBJ			=	$(SRC:.cpp=.o)

CXX			=	c++

CXXFLAGS	=	-Wall -Werror -Wextra	-std=c++98

all:		$(NAME)

$(NAME):	$(OBJ)
			$(CXX)	$(CXXFLAGS) $(OBJ)	-o $(NAME)	

clean:		
			/bin/rm -f $(OBJ)
				
fclean:		clean
			/bin/rm -f $(NAME)

re:			fclean	all

.PHONY:		all	clean	fclean	re
