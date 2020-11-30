# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/24 19:40:30 by dmilan            #+#    #+#              #
#    Updated: 2020/11/30 17:30:48 by dmilan           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= cub3D
FLAGS		= #-Wall -Wextra -Werror
MLX			= minilibx
LIB			= libft/libft.a
INC_DIRS	= -Iminilibx -I. -Ilibft

SRC_MAP		= \
			map/map.c \
			map/validate_map.c

SRC			= \
			$(SRC_MAP) \
			cub3d.c \
			color.c \
			vector.c \
			draw.c


OBJ			= $(SRC:.c=.o)
INC			= cub3d.h

all: $(NAME)

$(LIB):
	make -C libft

%.o: %.c
	 gcc $(FLAGS) $(INC_DIRS) -c $< -o $@

$(NAME): $(LIB) $(OBJ) $(INC)
	gcc -g $(FLAGS) $(SRC) libft/libft.a -Lminilibx -lmlx -framework OpenGL -framework AppKit -o $(NAME) -I.  # remove SRC and -g
	

clean:
	make clean -C libft
	rm -f $(OBJ)

fclean: clean
	make fclean -C libft
	rm -f $(NAME)

re: fclean all

run:
	./$(NAME) map/map.cub