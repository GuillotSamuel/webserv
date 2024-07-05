# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/05 13:36:24 by sguillot          #+#    #+#              #
#    Updated: 2024/07/05 14:11:37 by sguillot         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
CC = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD -I./includes
SRC =	$(wildcard ./srcs/**/*.cpp)
OBJDIR = ./obj
DEPDIR = ./dep
OBJ = $(patsubst ./srcs/%.cpp,$(OBJDIR)/%.o,$(SRC))
DEP = $(patsubst ./srcs/%.cpp,$(DEPDIR)/%.d,$(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

$(OBJDIR)/%.o: ./srcs/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) -c $< -o $@

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

clean:
	rm -f $(OBJ) $(DEP)

fclean: clean
	rm -f $(NAME)
	rm -rf $(OBJDIR) $(DEPDIR)

re: fclean all

-include $(DEP)

.PHONY: all clean fclean re
