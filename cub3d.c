/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 09:54:39 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/05 18:44:25 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	move(t_vars *vars, double rotation)
{
	vars->player.pos.x += rotate_vector(vars->player.direction, rotation).x
						* vars->player.speed;
	if (vars->map[(int)vars->player.pos.y][(int)vars->player.pos.x] == '1')
		vars->player.pos.x -= rotate_vector(vars->player.direction, rotation).x
							* vars->player.speed;
	vars->player.pos.y += rotate_vector(vars->player.direction, rotation).y
						* vars->player.speed;
	if (vars->map[(int)vars->player.pos.y][(int)vars->player.pos.x] == '1')
		vars->player.pos.y -= rotate_vector(vars->player.direction, rotation).y
							* vars->player.speed;
}

void	rotate(t_vars *vars, double rotation)
{
	vars->player.direction = rotate_vector(vars->player.direction, rotation);
	vars->player.plane = rotate_vector(vars->player.plane, rotation);
}

int		key_pressed(int keycode, t_vars *vars)
{
	if (keycode == W)
		move(vars, 0);
	else if (keycode == A)
		move(vars, -PI / 2);
	else if (keycode == S)
		move(vars, PI);
	else if (keycode == D)
		move(vars, PI / 2);
	else if (keycode == LEFT_ARROW)
		rotate(vars, -vars->player.turn_speed);
	else if (keycode == RIGHT_ARROW)
		rotate(vars, vars->player.turn_speed);
	else if (keycode == ESC)
	{
		mlx_destroy_image(vars->mlx, vars->frame.image);
		mlx_destroy_window(vars->mlx, vars->window);
		// free stuff
		exit(0);
	}
	return (1);  // what is return of this funcion?
}

void	draw_map(t_vars *vars)
{
	int		i;
	int		j;
	int		size;
	t_point	pos;

	i = -1;
	size = 10;
	while (++i < vars->y)
	{
		j = -1;
		while (++j < vars->x[i])
		{
			if (vars->map[i][j] == '1')
				draw_square(&vars->frame, j * size, i * size, size, 0x81000000);
			else if (vars->map[i][j] == '0')
				draw_square(&vars->frame, j * size, i * size, size, 0x81808080);
			else if (vars->map[i][j] == '2')
				draw_square(&vars->frame, j * size, i * size, size, 0x00FF1111);
			else if (ft_strchr("NEWS", vars->map[i][j]))
				draw_square(&vars->frame, j * size, i * size, size, 0x0011FF11);
		}
	}
	pos.x = vars->player.pos.x * size;
	pos.y = vars->player.pos.y * size;
	draw_circle(&vars->frame, pos, size / 4, 0x00E76F51);
}

void	calc_delta(t_ray *ray)
{
		ray->delta_v.x = (ray->x >= 0 ? 1 : -1);
		ray->delta_h.y = (ray->y >= 0 ? 1 : -1);
		ray->delta_v.y = (ray->x) ? ray->y / ray->x : 0;
		ray->delta_h.x = (ray->y) ? ray->x / ray->y : 0;
		if (ray->x < 0 && ray->delta_h.x > 0)
			ray->delta_h.x *= -1;
		if (ray->x > 0 && ray->delta_h.x < 0)
			ray->delta_h.x *= -1;
		if (ray->y < 0 && ray->delta_v.y > 0)
			ray->delta_v.y *= -1;
		if (ray->y > 0 && ray->delta_v.y < 0)
			ray->delta_v.y *= -1;
}

void	calc_side(t_ray *ray, t_vars *vars)
{
	if (ray->y < 0)
	{
		ray->side_h.y = -(vars->player.pos.y - ray->map_y);
		ray->side_h.x = (ray->side_h.y > 0 ? ray->side_h.y : -ray->side_h.y)
						* ray->delta_h.x;
	}
	else
	{
		ray->side_h.y = (ray->map_y + 1 - vars->player.pos.y);
		ray->side_h.x = (ray->side_h.y > 0 ? ray->side_h.y : -ray->side_h.y)
						* ray->delta_h.x;
	}
	if (ray->x < 0)
	{
		ray->side_v.x = -(vars->player.pos.x - ray->map_x);
		ray->side_v.y = (ray->side_v.x > 0 ? ray->side_v.x : -ray->side_v.x)
						* ray->delta_v.y;
	}
	else
	{
		ray->side_v.x = (ray->map_x + 1 - vars->player.pos.x);
		ray->side_v.y = (ray->side_v.x > 0 ? ray->side_v.x : -ray->side_v.x)
						* ray->delta_v.y;
	}
}

void	cast_ray(t_ray *ray, t_vars *vars)
{	
	ray->sprites = 0;
	while (1)
	{
		if (ray->x && vector_len(ray->side_v) < vector_len(ray->side_h))
		{
			ray->side = (ray->x > 0) ? 'e' : 'w';
			ray->map_x += (ray->x >= 0) ? 1 : -1;
			if (vars->map[ray->map_y][ray->map_x] == '2')
			{
				ray->sprites++;
			}
			if (vars->map[ray->map_y][ray->map_x] == '1')
				break;
			ray->side_v = ft_point_add(ray->side_v, ray->delta_v);
		}
		else
		{
			ray->side = (ray->y > 0) ? 's' : 'n';
			ray->map_y += (ray->y >= 0) ? 1 : -1;
			if (vars->map[ray->map_y][ray->map_x] == '2')
			{
				ray->sprites++;
			}
			if (vars->map[ray->map_y][ray->map_x] == '1')
				break;
			ray->side_h = ft_point_add(ray->side_h, ray->delta_h);
		}
	}
}

void	calc_wall(t_ray *ray, t_vars *vars)
{
	double	cos_angle;
	double	dist_to_wall_perp;
	
	ray->wall.x = (ray->side == 'w' || ray->side == 'e') ? ray->side_v.x : ray->side_h.x;
	ray->wall.y = (ray->side == 'w' || ray->side == 'e') ? ray->side_v.y : ray->side_h.y;
	cos_angle = (vars->player.plane.x * ray->wall.x + vars->player.plane.y * ray->wall.y) /
				(vector_len(ray->wall) * vector_len(vars->player.plane));
	dist_to_wall_perp = sqrt(1 - cos_angle * cos_angle) * vector_len(ray->wall);
	ray->wall_height = vars->resolution.y / dist_to_wall_perp;
	ray->wall_start = -ray->wall_height / 2 + vars->resolution.y / 2;
	ray->wall_end = ray->wall_height / 2 + vars->resolution.y / 2;
	if (ray->wall_end > vars->resolution.y)
		ray->wall_end = vars->resolution.y;
}

void	draw_sprites(t_ray *ray, t_vars *vars, int i)
{
	int		y;
	int		color;
	
	int		sprite_start;
	int		sprite_end;
	t_point	sprite;
	double	determinant;
	double	transform_x;
	double	transform_y;

	// sprite_x = ;
	// sprite_y = ;
	determinant = 1.0 / (vars->player.plane.x * vars->player.direction.y - vars->player.plane.y * vars->player.direction.x);
	transform_x = determinant * (vars->player.direction.y * sprite.x - vars->player.direction.x * sprite.y);
	transform_y = determinant * (-vars->player.plane.y * sprite.x + vars->player.plane.x * sprite.y);

	y = (sprite_start < 0) ? 0 : sprite_start;
	while (y < vars->resolution.y && y <= sprite_end)
	{
		color = 0xFF00FF; // calculates color using sprite_x and sprite_y;
		if (color != SPRITE_BG)
			put_pixel(&vars->frame, -i, y, color);
		y++;
	}
	
	
}

int		calc_texture_color(t_ray *ray, t_vars *vars, t_image texture, int y)
{
	int sprite_y;
	int sprite_x;

	sprite_y = (y * 1.0 - ray->wall_start) / ray->wall_height * texture.height;
	if (ray->side == 'w' || ray->side == 'e')
		sprite_x = (vars->player.pos.y + ray->wall.y - (int)(ray->wall.y + vars->player.pos.y)) * texture.width;
	else
		sprite_x = (vars->player.pos.x + ray->wall.x - (int)(ray->wall.x + vars->player.pos.x)) * texture.width;
	return (get_pixel(&texture, sprite_x, sprite_y));
}

void	draw_wall(t_ray *ray, t_vars *vars, int i)
{
	int	y;
		
	y = (ray->wall_start < 0) ? 0 : ray->wall_start;
	draw_line(&vars->frame, -i, 0, -i, (ray->wall_start < 0) ? 0 : ray->wall_start, vars->texture.ceilling.argb);
	while (y < vars->resolution.y && y <= ray->wall_end)
	{
		if (ray->side == 'n')
			put_pixel(&vars->frame, -i, y, calc_texture_color(ray, vars, vars->texture.north, y));
		else if (ray->side == 'e')
			put_pixel(&vars->frame, -i, y, calc_texture_color(ray, vars, vars->texture.east, y));
		else if (ray->side == 'w')
			put_pixel(&vars->frame, -i, y, calc_texture_color(ray, vars, vars->texture.west, y));
		else if (ray->side == 's')
			put_pixel(&vars->frame, -i, y, calc_texture_color(ray, vars, vars->texture.south, y));
		y++;
	}
	draw_line(&vars->frame, -i, vars->resolution.y, -i, ray->wall_end, vars->texture.floor.argb);
}

void	cast_rays(t_vars *vars)
{
	t_ray		ray;
	int			i;

	i = -1;
	while (++i <= vars->resolution.x)
	{
		ray.map_x = (int)vars->player.pos.x;
		ray.map_y = (int)vars->player.pos.y;
		ray.x = vars->player.direction.x + vars->player.plane.x
				* (i * 2.0 / vars->resolution.x - 1);
		ray.y = vars->player.direction.y + vars->player.plane.y
				* (i * 2.0 / vars->resolution.x - 1);

		calc_delta(&ray);
		calc_side(&ray, vars);
		cast_ray(&ray, vars);
		calc_wall(&ray, vars);
		draw_wall(&ray, vars, i);
	}
}



int		render_next_frame(t_vars *vars)
{
	cast_rays(vars);
	// cast_rays_sprite(vars);
	// draw_map(vars);
	// draw_sprites(vars);
	mlx_put_image_to_window(vars->mlx, vars->window, vars->frame.image, 0, 0);
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

	if (ft_strncmp(argv[1], "--save", 6) == 0)
	{
		return (0);
		// render and save image in bmp format.
	}
	if (!(vars = default_vars()))
		return (-1);
	if (!read_map("map/map.cub", vars))
		return (-1);
	if (!validate_map(vars))
		return (-1);
	ft_putcppn_fd(vars->map, vars->y, 1);

	vars->frame.image = mlx_new_image(vars->mlx, vars->resolution.x, vars->resolution.y);
	vars->frame.address = mlx_get_data_addr(vars->frame.image, &vars->frame.bits_per_pixel, &vars->frame.len, &vars->frame.endian);
	vars->window = mlx_new_window(vars->mlx, vars->resolution.x, vars->resolution.y, "kindow");
	mlx_hook(vars->window, 2, 1L << 0, key_pressed, vars);  // why mask ?
	mlx_loop_hook(vars->mlx, render_next_frame, vars);
	mlx_loop(vars->mlx);
	return (0);
}