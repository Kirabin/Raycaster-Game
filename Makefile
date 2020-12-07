# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/24 19:40:30 by dmilan            #+#    #+#              #
#    Updated: 2020/12/07 19:53:51 by dmilan           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= cub3D
FLAGS		= -Wall -Wextra #-Werror
LIB			= libft/libft.a

SRC_MAP		= \
				map.c \
				map_validate.c \
				map_parce.c

SRC			= \
				$(addprefix map/, $(SRC_MAP)) \
				cub3d.c \
				draw.c


OBJ			= $(SRC:.c=.o)
INC_DIRS	=  -I. -I./libft -I./mlx_opengl
HEADERS		= cub3d.h libft/libft.h

all: $(NAME)

$(LIB):
	make -C libft

%.o: %.c $(HEADERS)
	 gcc $(FLAGS) $(INC_DIRS) -c $< -o $@

$(NAME): $(LIB) $(OBJ) $(INC)
	gcc -g $(FLAGS) $(SRC) libft/libft.a -framework OpenGL -framework AppKit -o $(NAME) $(INC_DIRS) -Lminilibx -lmlx  #remove SRC and -g

clean:
	make clean -C libft
	rm -f $(OBJ)

fclean: clean
	make fclean -C libft
	rm -f $(NAME)

re: fclean all

run:
	./$(NAME) map/map.cub