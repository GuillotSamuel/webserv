# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/05 13:36:24 by sguillot          #+#    #+#              #
#    Updated: 2024/08/29 12:57:09 by mmahfoud         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
CC = c++
CPPFLAGS = -Wall -Wextra -Werror -g3 -std=c++98 -MMD -MP -I./includes
SRC =	$(wildcard $(shell find ./srcs -type f -name "*.cpp"))
OBJDIR = ./obj
DEPDIR = ./dep
OBJ = $(patsubst ./srcs/%.cpp,$(OBJDIR)/%.o,$(SRC))
DEP = $(patsubst ./srcs/%.cpp,$(DEPDIR)/%.d,$(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

$(OBJDIR)/%.o: ./srcs/%.cpp $(DEPDIR)/%.d | $(OBJDIR)
	@mkdir -p $(dir $@)
	@$(CC) $(CPPFLAGS) -c $< -o $@

$(DEPDIR)/%.d: ./srcs/%.cpp | $(DEPDIR)
	@mkdir -p $(dir $@)
	@$(CC) $(CPPFLAGS) -MM -MP -MF $@ -MT '$(OBJDIR)/$(<:./srcs/%.cpp=$(OBJDIR)/%.o)' $<

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(DEPDIR):
	@mkdir -p $(DEPDIR)

clean:
	@rm -f $(OBJ) $(DEP)

fclean: clean
	@rm -f $(NAME)
	@rm -rf $(OBJDIR) $(DEPDIR)

re: fclean all

-include $(DEP)

.PHONY: all clean fclean re




