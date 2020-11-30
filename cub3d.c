/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 09:54:39 by dmilan            #+#    #+#             */
/*   Updated: 2020/11/30 18:25:07 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdio.h> //delete


int		key_pressed(int keycode, t_vars *vars)
{
	
	// ft_printf("%d\n", keycode);
	if (keycode == 13)  // W
	{
		vars->player.position.x += vars->player.direction.x * vars->player.speed;
		if (vars->map[(int)vars->player.position.y][(int)vars->player.position.x] == '1')
			vars->player.position.x -= vars->player.direction.x * vars->player.speed;
		vars->player.position.y += vars->player.direction.y * vars->player.speed;
		if (vars->map[(int)vars->player.position.y][(int)vars->player.position.x] == '1')
			vars->player.position.y -= vars->player.direction.y * vars->player.speed;
	}
	else if (keycode == 0)  // A
	{
		vars->player.direction = rotate_vector(vars->player.direction, -vars->player.turn_speed);
		vars->player.plane = rotate_vector(vars->player.plane, -vars->player.turn_speed);
	}
	else if (keycode == 1)  // S
	{
		vars->player.position.x -= vars->player.direction.x * vars->player.speed;
		if (vars->map[(int)vars->player.position.y][(int)vars->player.position.x] == '1')
			vars->player.position.x += vars->player.direction.x * vars->player.speed;
		vars->player.position.y -= vars->player.direction.y * vars->player.speed;
		if (vars->map[(int)vars->player.position.y][(int)vars->player.position.x] == '1')
			vars->player.position.y += vars->player.direction.y * vars->player.speed;
		// printf("pos: %f %f\n", vars->player.position.x, vars->player.position.y);
	}
	else if (keycode == 2)  // D
	{
		vars->player.direction = rotate_vector(vars->player.direction, vars->player.turn_speed);
		vars->player.plane = rotate_vector(vars->player.plane, vars->player.turn_speed);
	}
	else if (keycode == 53)
	{
		mlx_destroy_window(vars->mlx, vars->window);
		// free stuff
	}
	return (1); // what is return of this funcion? 
}


void	draw_map(t_image *image, t_vars *vars)
{
	int		i;
	int		j;
	int		radius = 20;
	
	i = 0;
	while (i < vars->y)
	{
		j = 0;
		while (j < vars->x[i])
		{
			// ft_printf("%s\n", vars->map[i]);
			if (vars->map[i][j] == '1')
			{
				draw_square(image, j * radius + 1, i * radius + 1, radius - 2, 0x00FFA700);
			}
			// else if (vars->map[i][j] == '0')
			// 	draw_square(image, j * radius + 1, i * radius + 1, radius - 2, 0x00000000);
			else if (ft_strchr("NEWS", vars->map[i][j]))
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
	while (i < vars->resolution.x)
	{
		ray.x = vars->player.direction.x + vars->player.plane.x * (i * 2.0 / vars->resolution.x - 1);
		ray.y = vars->player.direction.y + vars->player.plane.y * (i * 2.0 / vars->resolution.x - 1);

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
				side = (ray.x > 0) ? 'e' : 'w';
				map_x += step.x;
				if (vars->map[map_y][map_x] == '1')
					break;
				side_v = ft_point_add(side_v, delta_v);
			}
			// else if both equal
			else
			{
				side = (ray.y > 0) ? 's' : 'n';
				map_y += step.y;
				if (vars->map[map_y][map_x] == '1')
					break;
				side_h = ft_point_add(side_h, delta_h);
			}
		}

		t_point		wall;
		if (side == 'w' || side == 'e')
		{
			wall.x = side_v.x;
			wall.y = side_v.y;
		}
		else 
		{
			wall.x = side_h.x;
			wall.y = side_h.y;
		}
	
		// t_point from;
		// t_point to;
		// from.x = vars->player.position.x * 20;
		// from.y = vars->player.position.y * 20;
		// to.x = from.x + wall.x * 20;
		// to.y = from.y + wall.y * 20;
		// draw_line(image, (int)from.x, (int)from.y, (int)to.x, (int)to.y, 0x00FFFFFF);
		
		
		float wall_dist;
		float cos_angle;
		float sin_angle;
		float wall_dist_perp;
		wall_dist = vector_len(wall);
		cos_angle = (vars->player.plane.x * wall.x + vars->player.plane.y * wall.y) /
					(wall_dist * vector_len(vars->player.plane));
		sin_angle = sqrt(1 - cos_angle * cos_angle);
		wall_dist_perp = sin_angle * wall_dist;
		

		int		line_height = (int)(vars->resolution.y / wall_dist_perp); // wall_dist_perp;
		int		line_start_y;
		int		line_end_y;
		
		line_start_y = -line_height / 2 + vars->resolution.y / 2;
		if (line_start_y < 0)
			line_start_y = 0;
		line_end_y = line_height / 2 + vars->resolution.y / 2;
		if (line_end_y > vars->resolution.y)
			line_end_y = vars->resolution.y - 5;
		
		
		t_color north_color;
		north_color.r = 0xed; 
		north_color.g = 0x55;
		north_color.b = 0x3b;
		north_color.argb = argb_color(0, north_color.r, north_color.g, north_color.b);
		
		t_color east_color;  //0x00F4A200
		east_color.r = 0xf6;
		east_color.g = 0xd5;
		east_color.b = 0x5c;
		east_color.argb = argb_color(0, east_color.r, east_color.g, east_color.b);
		
		t_color west_color;
		west_color.r = 0x3C;
		west_color.g = 0xAE;
		west_color.b = 0xA3;
		west_color.argb = argb_color(0, west_color.r, west_color.g, west_color.b);
		
		t_color south_color;
		south_color.r = 0x20;
		south_color.g = 0x63;
		south_color.b = 0x9B;
		south_color.argb = argb_color(0, south_color.r, south_color.g, south_color.b);
		
		if (side == 'n')
		{
			draw_line_gradient(image, -i, 0, -i, line_start_y, vars->texture.ceilling);
			draw_line(image, -i, line_start_y, -i, line_end_y, dim_color(north_color, wall_dist_perp * 8));
			draw_line_gradient(image, -i, vars->resolution.y, -i, line_end_y, vars->texture.floor);
		}
		else if (side == 'e')
		{
			draw_line_gradient(image, -i, 0, -i, line_start_y, vars->texture.ceilling);
			draw_line(image, -i, line_start_y, -i, line_end_y, dim_color(east_color, wall_dist_perp * 8));
			draw_line_gradient(image, -i, vars->resolution.y, -i, line_end_y, vars->texture.floor);
		}
		else if (side == 'w')
		{
			draw_line_gradient(image, -i, 0, -i, line_start_y, vars->texture.ceilling);
			draw_line(image, -i, line_start_y, -i, line_end_y, dim_color(west_color, wall_dist_perp * 8));
			draw_line_gradient(image, -i, vars->resolution.y, -i, line_end_y, vars->texture.floor);
		}
		else if (side == 's')
		{
			draw_line_gradient(image, -i, 0, -i, line_start_y, vars->texture.ceilling);
			draw_line(image, -i, line_start_y, -i, line_end_y, dim_color(south_color, wall_dist_perp * 8));
			draw_line_gradient(image, -i, vars->resolution.y, -i, line_end_y, vars->texture.floor);
		}
		i++;
	}
}

int		render_next_frame(t_vars *vars)
{

	cast_rays(&vars->frame, vars);
	// draw_map(&image, vars);

	mlx_put_image_to_window(vars->mlx, vars->window, vars->frame.image, 0, 0);
	// mlx_destroy_image(vars->mlx, vars->frame.image);
	return (1); // what is return of this funcion?
}

t_vars	*default_vars(void)
{
	t_vars	*vars;

	if (!(vars = malloc(sizeof(t_vars))))
		return (0);
	vars->y = 0;
	vars->player.speed = 0.15;
	vars->player.turn_speed = PI / 30;
	vars->mlx = mlx_init();
	return (vars);
}

int		main(int argc, char **argv)
{
	t_vars	*vars;

	if (!(vars = default_vars()))
		return (-1);
	if (!read_map(argv[1], vars))
		return (-1);
	if (!validate_map(vars))
		return (-1);
	ft_putcppn_fd(vars->map, vars->y, 1);

	
	vars->frame.image = mlx_new_image(vars->mlx, vars->resolution.x, vars->resolution.y);
	vars->frame.address = mlx_get_data_addr(vars->frame.image, &vars->frame.bits_per_pixel, &vars->frame.len, &vars->frame.endian);
	
	vars->window = mlx_new_window(vars->mlx, vars->resolution.x, vars->resolution.y, "kindow");
	mlx_hook(vars->window, 2, 1L<<0, key_pressed, vars);  // why mask ? 
	mlx_loop_hook(vars->mlx, render_next_frame, vars);
	mlx_loop(vars->mlx);

	return (0);
}
