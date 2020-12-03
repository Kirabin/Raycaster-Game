/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 09:55:50 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/03 16:25:51 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# include "libft/libft.h"
# include <math.h>
# include <mlx.h>
# define PI 3.14159265359
# define W 13
# define A 0
# define S 1
# define D 2
# define LEFT_ARROW 123
# define RIGHT_ARROW 124
# define ESC 53


typedef struct		s_color
{
	int				a;
	int				r;
	int				g;
	int				b;
	int				argb;
}					t_color;

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

typedef struct		s_texture
{
	t_image			north;
	t_image			east;
	t_image			south;
	t_image			west;
	t_image			sprite;
	t_color			floor;
	t_color			ceilling;
}					t_texture;

typedef struct		s_player
{
	t_point			pos;
	t_point			direction;
	t_point			plane;
	double			speed;
	double			turn_speed;
}					t_player;

typedef struct		s_ray
{
	double			x;
	double			y;
	t_point			delta_v;
	t_point			delta_h;
	t_point			side_v;
	t_point			side_h;
	t_point			wall;
	int				wall_height;
	int				wall_start;
	int				wall_end;
	char			side;
	int				map_x;
	int				map_y;
}					t_ray;

typedef struct		s_vars
{
	void			*mlx;
	void			*window;
	char			**map;
	t_player		player;
	t_image			frame;
	t_point			resolution;
	int				y;
	int				*x;
	t_texture		texture;
}					t_vars;

int					read_map(char *file, t_vars *vars);
int					validate_map(t_vars *vars);

/*
**  color.c
*/
t_color				parce_color(const char *line);
int					dim_color(t_color color, int value);
int					argb_color(int a, int r, int g, int b);
// void				put_pixel_from_texture(t_image *frame, int x1, int y1, t_image *texture, int x2, int y2);


/*
**  vector.c
*/
t_point				rotate_vector(t_point point, double angle);
double				vector_len(t_point point);

/*
**  draw.c
*/
void				put_pixel(t_image *image, int x, int y, int color);
int					get_pixel(t_image *image, int x, int y);
void				draw_line(t_image *image, int x1, int y1, int x2, int y2, int color);
void				draw_circle(t_image *image, t_point center, int radius, int color);
void				draw_square(t_image *image, int x, int y, int radius, int color);
void				draw_rectangle(t_image *image, int x1, int y1, int x2, int y2, int color);
void				draw_line_gradient(t_image *image, int x1, int y1, int x2, int y2, t_color color);



#endif
