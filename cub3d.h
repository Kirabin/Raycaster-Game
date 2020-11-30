/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 09:55:50 by dmilan            #+#    #+#             */
/*   Updated: 2020/11/30 11:23:18 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# include "libft/libft.h"
# include <math.h>
# include <mlx.h>
# define PI 3.14159265359


typedef struct		s_color
{
	int				a;
	int				r;
	int				g;
	int				b;
	int				argb;
}					t_color;

typedef struct		s_texture
{
	char			*north;
	char			*east;
	char			*south;
	char			*west;
	char			*sprite;
	t_color			floor;
	t_color			ceilling;
}					t_texture;

typedef struct		s_map
{
	char			**field;
	t_point			resolution;
	int				y;
	int				*x;
	t_texture		texture;
}					t_map;

typedef struct		s_image
{
	void			*image;
	char			*address; // ?
	int				bits_per_pixel;
	int				len;
	int				endian; // ?
	int				width;
	int				height;
	
}					t_image;

typedef struct		s_player
{
	t_point			position;
	t_point			direction;
	t_point			plane;
	double			speed;
}					t_player;

typedef struct		s_vars
{
	t_map			*map;
	void			*mlx;
	void			*window;
	t_player		player;
}					t_vars;

int					read_map(char *file, t_vars *vars);
int					validate_map(t_map *map);

#endif
