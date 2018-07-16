# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: logan  <logan@42.us.org>                   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/13 10:03:24 by logan             #+#    #+#              #
#    Updated: 2018/07/14 20:50:45 by twalton          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = RT
LIST = main \
Camera \
ImagePipeline \
Plane \
Scene \
Sphere \
Cylinder \
Cone \
Sampler \
IObject

SRC_DIR = src
OBJ_DIR = obj

SRC = $(addsuffix .cpp, $(addprefix $(SRC_DIR), $(LIST)))
OBJ = $(addsuffix .o, $(addprefix $(OBJ_DIR)/, $(LIST)))
DEP = $(OBJ:%.o=%.d)

CPPFLAGS = -std=c++14 -Wall -Wextra -Werror -Wno-unused-parameter\
$(shell PKG_CONFIG_PATH=~/.brew/opt/qt/lib/pkgconfig \
pkg-config --cflags glfw3 glm Qt5Core Qt5Gui Qt5Widgets) \
-I lib/lodepng \
-O3 -flto=thin \
#-g -fsanitize=undefined -fsanitize=address

LDFLAGS = -framework OpenGl \
$(shell PKG_CONFIG_PATH=~/.brew/opt/qt/lib/pkgconfig \
pkg-config --libs glfw3 glm Qt5Core Qt5Gui Qt5Widgets) \
-L lib/lodepng -llodepng -flto=thin \
#-fsanitize=undefined -fsanitize=address

all: $(OBJ_DIR) $(NAME)

$(NAME): lib/lodepng/liblodepng.a $(OBJ)
	@printf "\e[32;1mLinking.. \e[0m\n"
	@export PKG_CONFIG_PATH=/usr/local/opt/qt/lib/pkgconfig
	@clang++ $(LDFLAGS) -o $@ $^
	@printf "\e[32;1mCreated:\e[0m %s\n" $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

-include $(DEP)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@printf "\e[34;1mCompiling: \e[0m%s\n" $<
	@clang++ $(CPPFLAGS) -MMD -c $< -o $@

lib/lodepng/liblodepng.a: lib/lodepng/lodepng.cpp
	@printf "\e[35;1mCompiling Dependency: \e[0m%s\n" $<
	@clang++ $(CPPFLAGS) -c -o lib/lodepng/lodepng.o $<
	@ar rcs $@ lib/lodepng/lodepng.o

clean:
	@printf "\e[31;1mCleaning..\e[0m\n"
	@rm -f $(OBJ)
	@rm -f lib/lodepng/lodepng.o

fclean:
	@printf "\e[31;1mFull Cleaning..\e[0m\n"
	@rm -rf $(OBJ_DIR)
	@rm -f $(NAME)
	@rm -f lib/lodepng/liblodepng.a

re:	fclean all

deps:
	@./deps.sh

.PHONY: clean fclean all re docs
