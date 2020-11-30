/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 09:54:39 by dmilan            #+#    #+#             */
/*   Updated: 2020/11/30 11:47:49 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdio.h> //delete

void	put_pixel(t_image *image, int x, int y, int color)
{
	char	*dst;
	
	dst = image->address + (y * image->len + x * (image->bits_per_pixel / 8));  // why offset;
	*(unsigned int *)dst = color;
}

t_point	rotate_vector(t_point point, double angle)
{
	t_point	result;

	result.x = point.x * cos(angle) - point.y * sin(angle);
	result.y = point.x * sin(angle) + point.y * cos(angle);
	return (result);
}

double	vector_len(t_point point)
{
	return (sqrt(point.x * point.x + point.y * point.y));
}

int		key_pressed(int keycode, t_vars *vars)
{
	
	// ft_printf("%d\n", keycode);
	if (keycode == 13)  // W
	{
		vars->player.position.x += vars->player.direction.x * vars->player.speed;
		if (vars->map->field[(int)vars->player.position.y][(int)vars->player.position.x] == '1')
			vars->player.position.x -= vars->player.direction.x * vars->player.speed;
		vars->player.position.y += vars->player.direction.y * vars->player.speed;
		if (vars->map->field[(int)vars->player.position.y][(int)vars->player.position.x] == '1')
			vars->player.position.y -= vars->player.direction.y * vars->player.speed;
	}
	else if (keycode == 0)  // A
	{
		vars->player.direction = rotate_vector(vars->player.direction, -PI / 20);
		vars->player.plane = rotate_vector(vars->player.plane, -PI / 20);
	}
	else if (keycode == 1)  // S
	{
		vars->player.position.x -= vars->player.direction.x * vars->player.speed;
		if (vars->map->field[(int)vars->player.position.y][(int)vars->player.position.x] == '1')
			vars->player.position.x += vars->player.direction.x * vars->player.speed;
		vars->player.position.y -= vars->player.direction.y * vars->player.speed;
		if (vars->map->field[(int)vars->player.position.y][(int)vars->player.position.x] == '1')
			vars->player.position.y += vars->player.direction.y * vars->player.speed;
		// printf("pos: %f %f\n", vars->player.position.x, vars->player.position.y);
	}
	else if (keycode == 2)  // D
	{
		vars->player.direction = rotate_vector(vars->player.direction, PI / 20);
		vars->player.plane = rotate_vector(vars->player.plane, PI / 20);
	}
	else if (keycode == 53)
	{
		mlx_destroy_window(vars->mlx, vars->window);
		// free stuff
	}
	return (1); // what is return of this funcion? 
}

void	draw_line(t_image *image, int x1, int y1, int x2, int y2, int color)
{
	t_point	delta;
	float			ratio;
	int				x;
	int				y;

	ratio = 0;
	delta.x = x2 - x1;
	delta.y = y2 - y1;
	if (ft_absi(delta.y) < ft_absi(delta.x))
	{
		x = 0;
		while (x <= ft_absi(delta.x))
		{
			ratio = x * 1.0 / ft_absi(delta.x);
			put_pixel(image, x1 + x * (delta.x > 0 ? 1 : -1), y1 + ratio * ft_absi(delta.y) * (delta.y > 0 ? 1 : -1), color);
			x++;
		}
	}
	else
	{
		y = 0;
		while (y <= ft_absi(delta.y))
		{
			ratio = y * 1.0 / ft_absi(delta.y);
			put_pixel(image, x1 + ratio * ft_absi(delta.x) * (delta.x > 0 ? 1 : -1), y1 + y * (delta.y > 0 ? 1 : -1), color);
			y++;
		}
	}
}

void	draw_circle(t_image *image, t_point center, int radius, int color)  // O(n^2)
{
	int		x;
	int		y;
	
	put_pixel(image, center.x, center.y, 0x0000FF00);
	x = -radius;
	while (x <= radius)
	{
		y = -radius;
		while (y <= radius)
		{
			if (x * x + y * y <= radius * radius)
				put_pixel(image, center.x + x, center.y + y, color);
			y++;
		}
		x++;
	}
}

void	draw_square(t_image *image, int x, int y, int radius, int color)
{
	int		i;
	int		j;
	
	i = x;
	while (i <= radius + x)
	{
		j = y;
		while (j <= radius + y)
		{
			put_pixel(image, i, j, color);
			j++;
		}
		i++;
	}
}

void	draw_map(t_image *image, t_vars *vars)
{
	int		i;
	int		j;
	int		radius = 20;
	
	i = 0;
	while (i < vars->map->y)
	{
		j = 0;
		while (j < vars->map->x[i])
		{
			// ft_printf("%s\n", vars->map->field[i]);
			if (vars->map->field[i][j] == '1')
			{
				draw_square(image, j * radius + 1, i * radius + 1, radius - 2, 0x00FFA700);
			}
			// else if (vars->map->field[i][j] == '0')
			// 	draw_square(image, j * radius + 1, i * radius + 1, radius - 2, 0x00000000);
			else if (ft_strchr("NEWS", vars->map->field[i][j]))
			{
				draw_square(image, j * radius + 1, i * radius + 1, radius - 2, 0x0011FF11);
			}
			
			j++;
		}
		i++;
	}
	t_point pos;
	
	pos.x = vars->player.position.x * 20;
	pos.y = vars->player.position.y * 20;
	draw_circle(image, pos, 5, 0x00E76F51);
}


void	cast_rays(t_image *image, t_vars *vars)
{
	t_point ray;
	t_point step;
	t_point	delta_v;
	t_point	delta_h;
	t_point	side_v;
	t_point	side_h;
	float	i;
	char	side;
	
	int		map_x;
	int		map_y;
	
	i = 0;

	while (i < vars->map->resolution.x)
	{
		ray.x = vars->player.direction.x + vars->player.plane.x * (i * 2.0 / vars->map->resolution.x - 1);
		ray.y = vars->player.direction.y + vars->player.plane.y * (i * 2.0 / vars->map->resolution.x - 1);
		
		delta_v.x = (ray.x >= 0 ? 1 : -1);
		delta_h.y = (ray.y >= 0 ? 1 : -1);
		delta_v.y = (ray.x) ? ray.y / ray.x : 0;
		delta_h.x = (ray.y) ? ray.x / ray.y : 0;
		if (ray.x < 0 && delta_h.x > 0)
			delta_h.x *= -1;
		if (ray.x > 0 && delta_h.x < 0)
			delta_h.x *= -1;
		if (ray.y < 0 && delta_v.y > 0)
			delta_v.y *= -1;
		if (ray.y > 0 && delta_v.y < 0)
			delta_v.y *= -1;


		map_x = (int)vars->player.position.x;
		map_y = (int)vars->player.position.y;
		if (ray.y < 0)
		{
			step.y = -1;
			side_h.y = -(vars->player.position.y - map_y);
			side_h.x = (side_h.y > 0 ? side_h.y : -side_h.y) * delta_h.x;
		}
		else
		{
			step.y = 1;
			side_h.y = (map_y + 1 - vars->player.position.y);
			side_h.x = (side_h.y > 0 ? side_h.y : -side_h.y) * delta_h.x;
		}
		
		int dont_go_v = 0;
		if (ray.x == 0)
		{
			dont_go_v = 1;
		}
		if (ray.x < 0)
		{
			step.x = -1;
			side_v.x = -(vars->player.position.x - map_x);
			side_v.y = (side_v.x > 0 ? side_v.x : -side_v.x) * delta_v.y; // no abs
		}
		else
		{
			step.x = 1;
			side_v.x = (map_x + 1 - vars->player.position.x);
			side_v.y = (side_v.x > 0 ? side_v.x : -side_v.x) * delta_v.y;
		}
		
		while (1)
		{
			if (!dont_go_v && vector_len(side_v) < vector_len(side_h))
			{
				side = 'v';
				map_x += step.x;
				if (vars->map->field[map_y][map_x] == '1')
					break;
				side_v = ft_point_add(side_v, delta_v);
			}
			// else if both equal
			else
			{
				side = 'h';
				map_y += step.y;
				if (vars->map->field[map_y][map_x] == '1')
					break;
				side_h = ft_point_add(side_h, delta_h);
			}
		}

		t_point		wall;
		if (side == 'v')
		{
			wall.x = side_v.x;
			wall.y = side_v.y;
		}
		else
		{
			wall.x = side_h.x;
			wall.y = side_h.y;
		}
	
		t_point from;
		t_point to;
		from.x = vars->player.position.x * 20;
		from.y = vars->player.position.y * 20;
		to.x = from.x + wall.x * 20;
		to.y = from.y + wall.y * 20;
		draw_line(image, (int)from.x, (int)from.y, (int)to.x, (int)to.y, 0x00FFFFFF);
		
		
		float wall_dist;
		float cos_angle;
		float sin_angle;
		float wall_dist_perp;
		wall_dist = vector_len(wall);
		cos_angle = (vars->player.plane.x * wall.x + vars->player.plane.y * wall.y) /
					(wall_dist * vector_len(vars->player.plane));
		sin_angle = sqrt(1 - cos_angle * cos_angle);
		wall_dist_perp = sin_angle * wall_dist;
		

		int		line_height = (int)(vars->map->resolution.y / wall_dist_perp); // wall_dist_perp;
		int		line_start_y;
		int		line_end_y;
		
		line_start_y = -line_height / 2 + vars->map->resolution.y / 2;
		if (line_start_y < 0)
			line_start_y = 0;
		line_end_y = line_height / 2 + vars->map->resolution.y / 2;
		if (line_end_y > vars->map->resolution.y)
			line_end_y = vars->map->resolution.y - 5;
		
		draw_line(image, -i, line_start_y, -i, line_end_y, (side == 'h') ? 0x00F4A200 : 0x00F19202);

		i++;
	}
}

void	draw_rectangle(t_image *image, int x1, int y1, int x2, int y2, int color)
{
	int		i;
	int		j;
	
	i = 1;
	while (i <= x2)
	{
		j = y1;
		while (j <= y2)
		{
			put_pixel(image, i, j, color);
			j++;
		}
		i++;
	}
}

int		render_next_frame(t_vars *vars)
{
	t_image	image;
	t_image walls;

	t_point step_to;

	walls.image = mlx_xpm_file_to_image(vars->mlx, "images/walls.xpm", &walls.width, &walls.height);
	image.image = mlx_new_image(vars->mlx, vars->map->resolution.x, vars->map->resolution.y); // change name of map-> resolutiodn
	image.address = mlx_get_data_addr(image.image, &image.bits_per_pixel, &image.len, &image.endian);

	draw_rectangle(&image, 0, 0, vars->map->resolution.x, vars->map->resolution.y / 2, vars->map->texture.ceilling.argb);
	draw_rectangle(&image, 0, vars->map->resolution.y / 2, vars->map->resolution.x, vars->map->resolution.y - 1, vars->map->texture.floor.argb);
	cast_rays(&image, vars);
	draw_map(&image, vars);

	mlx_put_image_to_window(vars->mlx, vars->window, image.image, 0, 0);
	// mlx_put_image_to_window(vars->mlx, vars->window, walls.image, 0, 0);
	mlx_destroy_image(vars->mlx, image.image);
	return (1); // what is return of this funcion? 	
}

int		main(int argc, char **argv)
{

	char	*file;
	t_vars	vars;
	int		i;

	i = 0;
	vars.player.speed = 0.15;
	if (!(read_map(argv[1], &vars)))
		return (-1);
	if (!validate_map(vars.map))
		return (-1);
	ft_putcppn_fd(vars.map->field, vars.map->y, 1);
	vars.mlx = mlx_init();
	vars.window = mlx_new_window(vars.mlx, vars.map->resolution.x, vars.map->resolution.y, "kindow");

	mlx_hook(vars.window, 2, 1L<<0, key_pressed, &vars);  // why mask ? 
	mlx_loop_hook(vars.mlx, render_next_frame, &vars);
	mlx_loop(vars.mlx);

	return (0);
}
