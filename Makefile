# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/24 19:40:30 by dmilan            #+#    #+#              #
#    Updated: 2020/12/19 17:07:10 by dmilan           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	cub3D
FLAGS		=	-Wall -Wextra #-Werror
LIBFT		=	libft/libft.a

SRC			=	cub3d.c \
				draw.c \
				draw_sprites.c \
				draw_wall.c \
				raycasting.c \
				raycasting_support.c \
				bindings.c \
				map.c \
				map_validate.c \
				map_validate_2.c \
				map_parce.c \
				bmp.c

OBJ			= $(SRC:.c=.o)
INC_DIRS	=  -I. -I./libft
HEADERS		= cub3d.h libft/libft.h
MLX_OG		= -framework OpenGL -framework AppKit -lmlx -I./mlx_opengl
MLX_MM		= -framework OpenGL -framework AppKit -L. -lmlx -I./mlx_mms

all: $(NAME)

$(LIBFT): 
	make -C libft

%.o: %.c $(HEADERS)
	 gcc $(FLAGS) $(INC_DIRS) -c $< -o $@

$(NAME): $(LIBFT) $(OBJ) $(INC)
	make -C mlx_mms
	mv mlx_mms/libmlx.dylib libmlx.dylib
	gcc -g $(FLAGS) $(SRC) $(MLX_MM) -L./libft -lft -o $(NAME) $(INC_DIRS)    #remove SRC and -g

clean:
	mv libmlx.dylib mlx_mms/libmlx.dylib
	make clean -C mlx_mms; cd ..
	make clean -C libft
	rm -f $(OBJ)

fclean: clean
	make fclean -C libft
	rm -f $(NAME)

re: fclean all

run:
	./$(NAME) levels/default.cub
	
save:
	./$(NAME) levels/default.cub --save