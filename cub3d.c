/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 09:54:39 by dmilan            #+#    #+#             */
/*   Updated: 2020/11/29 15:11:27 by dmilan           ###   ########.fr       */
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
		printf("pos: %f %f\n", vars->player.position.x, vars->player.position.y);
	}
	else if (keycode == 0)  // A
	{
		vars->player.direction = rotate_vector(vars->player.direction, -PI / 20);
	}
	else if (keycode == 1)  // S
	{
		vars->player.position.x -= vars->player.direction.x * vars->player.speed;
		if (vars->map->field[(int)vars->player.position.y][(int)vars->player.position.x] == '1')
			vars->player.position.x += vars->player.direction.x * vars->player.speed;
		vars->player.position.y -= vars->player.direction.y * vars->player.speed;
		if (vars->map->field[(int)vars->player.position.y][(int)vars->player.position.x] == '1')
			vars->player.position.y += vars->player.direction.y * vars->player.speed;
		printf("pos: %f %f\n", vars->player.position.x, vars->player.position.y);
	}
	else if (keycode == 2)  // D
	{
		vars->player.direction = rotate_vector(vars->player.direction, PI / 20);
	}
	else if (keycode == 53)
	{
		mlx_destroy_window(vars->mlx, vars->window);
		// free stuff
	}
	return (1); // what is return of this funcion? 
}

void	draw_line(t_image *image, t_point from, t_point to, int color)
{
	t_point	delta;
	float			ratio;
	int				x;
	int				y;

	ratio = 0;
	delta.x = to.x - from.x;
	delta.y = to.y - from.y;
	if (ft_absi(delta.y) < ft_absi(delta.x))
	{
		x = 0;
		while (x <= ft_absi(delta.x))
		{
			ratio = x * 1.0 / ft_absi(delta.x);
			put_pixel(image, from.x + x * (delta.x > 0 ? 1 : -1), from.y + ratio * ft_absi(delta.y) * (delta.y > 0 ? 1 : -1), color);
			x++;
		}
	}
	else
	{
		y = 0;
		while (y <= ft_absi(delta.y))
		{
			ratio = y * 1.0 / ft_absi(delta.y);
			put_pixel(image, from.x + ratio * ft_absi(delta.x) * (delta.x > 0 ? 1 : -1), from.y + y * (delta.y > 0 ? 1 : -1), color);
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

void	draw_map(t_image *image, t_map *map)
{
	int		i;
	int		j;
	int		radius = 20;
	
	i = 0;
	while (i < map->y)
	{
		j = 0;
		while (j < map->x[i])
		{
			// ft_printf("%s\n", map->field[i]);
			if (map->field[i][j] == '1')
			{
				draw_square(image, j * radius + 1, i * radius + 1, radius - 2, 0x00F4A200);
			}
			else if (ft_strchr("NEWS", map->field[i][j]))
			{
				draw_square(image, j * radius + 1, i * radius + 1, radius - 2, 0x0011FF11);
			}
			
			j++;
		}
		i++;
	}
}

void	draw_player(t_image *image, t_vars *vars)
{
	int		radius = 20;
	t_point pos;
	
	pos.x = vars->player.position.x * 20;
	pos.y = vars->player.position.y * 20;
	draw_circle(image, pos, 5, 0x00E76F51);
}


void	cast_rays(t_image *image, t_vars *vars)
{
	t_point ray;
	t_point step;
	float	ray_len;
	float	len_of_y_delta;
	float	len_of_x_delta;
	float	len_to_y_side;
	float	len_to_x_side;
	float	i;
	int		side;
	float		len_to_wall;
	
	int		map_x;
	int		map_y;
	
	i = 0;
	map_x = (int)vars->player.position.x;
	map_y = (int)vars->player.position.y;
	vars->player.plane.x = 0;
	vars->player.plane.y = -1;
	while (i < PI / 3)
	{
		len_of_y_delta = 0;
		len_of_x_delta = 0;
		// ray = ft_point_add(vars->player.direction, rotate_vector(vars->player.plane, 0.3));
		ray = rotate_vector(vars->player.direction, i);
		// ray = vars->player.direction;
		ray_len = sqrt(ray.x * ray.x + ray.y * ray.y);
		if (ray.y)
			len_of_y_delta = (ray.y > 0) ? ray_len / ray.y : -ray_len / ray.y;
		if (ray.x)
			len_of_x_delta = (ray.x > 0) ? ray_len / ray.x : -ray_len / ray.x;
		
		step.x = (ray.x < 0) ? -1 : 1;
		step.y = (ray.y < 0) ? -1 : 1;
		if (ray.x < 0)
			len_to_x_side = (vars->player.position.x - map_x) * len_of_x_delta;
		else
			len_to_x_side = (map_x + 1.0 - vars->player.position.x) * len_of_x_delta;
		if (ray.y < 0)
			len_to_y_side = (vars->player.position.y - map_y) * len_of_y_delta;
		else
			len_to_y_side = (map_y + 1.0 - vars->player.position.y) * len_of_y_delta;
		
		// printf("len: %f %f\n", len_to_x_side, len_to_y_side);
		// DDA
		while (1)
		{
			if (len_to_x_side < len_to_y_side)
			{
				len_to_x_side += len_of_x_delta;
				map_x += step.x;
				side = 0;
			}
			else
			{
				len_to_y_side += len_of_y_delta;
				map_y += step.y;
				side = 1;
			}
			if (map_y >= vars->map->y || map_x >= vars->map->x[map_y] ||
				map_y < 0 || map_x < 0) //delete
			{
				map_y = (int)vars->player.position.y;
				map_x = (int)vars->player.position.x;
				break;
			}
			if (vars->map->field[map_y][map_x] == '1')
				break;
		}
		float len_len;
		if (side)
		{
			len_to_wall = (map_y - vars->player.position.y + (1 - step.y) / 2) / ray.y; // ?
			len_len = len_to_y_side - len_of_y_delta ;
		}
		else
		{
			len_to_wall = (map_x - vars->player.position.x + (1 - step.x) / 2) / ray.x; // ?
			len_len = len_to_x_side - len_of_x_delta ;
		}
	
		t_point pos_p;
		pos_p.x = vars->player.position.x * 20;
		pos_p.y = vars->player.position.y * 20;
		t_point to;
		to.x = pos_p.x + (len_len * ray.x) / ray_len * 20;
		to.y = pos_p.y + (len_len * ray.y) / ray_len * 20;
		draw_line(image, pos_p, to, 0x00FFFFFF);
		
		
		i += PI / 180;
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


	draw_map(&image, vars->map);
	draw_player(&image, vars);

	// step_to.x = vars->player.position.x + vars->player.direction.x * 100;
	// step_to.y = vars->player.position.y + vars->player.direction.y * 100;
	// draw_line(&image, vars->player.position, step_to, 0x00F4A261);
	
	// rotating clockwise
	// step_to = rotate_vector(vars->player.position, step_to, 0.3);
	// draw_line(&image, vars->player.position, step_to, 0x00FF0000);
	
	// rotating counter clockwise
	// step_to = rotate_vector(vars->player.position, step_to, -0.6);
	// draw_line(&image, vars->player.position, step_to, 0x00FF0000);
	cast_rays(&image, vars);
	
	// draw_circle(&image, vars->player.position, 5, 0x00E76F51);
	
	// mlx_string_put(vars->mlx, vars->window, 100, 100, 0x00E76F51, "hello");
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
