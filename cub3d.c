/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 09:54:39 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/11 11:01:02 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int			dim_color(t_color color, int value)
{
	t_color new_color;
	
	new_color.r = (value > color.r) ? 0 : color.r - value;
	new_color.g = (value > color.g) ? 0 : color.g - value;
	new_color.b = (value > color.b) ? 0 : color.b - value;
	new_color.argb = ft_color_argb(0, new_color.r, new_color.g, new_color.b);
	
	return (new_color.argb);
}

void	move(t_vars *vars, double rotation)
{
	vars->player.pos.x += ft_point_rotate(vars->player.direction, rotation).x
						* vars->player.speed;
	if (vars->map[(int)vars->player.pos.y][(int)vars->player.pos.x] == '1')
		vars->player.pos.x -= ft_point_rotate(vars->player.direction, rotation).x
							* vars->player.speed;
	vars->player.pos.y += ft_point_rotate(vars->player.direction, rotation).y
						* vars->player.speed;
	if (vars->map[(int)vars->player.pos.y][(int)vars->player.pos.x] == '1')
		vars->player.pos.y -= ft_point_rotate(vars->player.direction, rotation).y
							* vars->player.speed;
}

void	rotate(t_vars *vars, double rotation)
{
	vars->player.direction = ft_point_rotate(vars->player.direction, rotation);
	vars->player.plane = ft_point_rotate(vars->player.plane, rotation);
}

void	ft_lstclear_ab(t_list **lst, void (*del)(void*))
{
	t_list	*node;
	t_list	*temp;

	node = *lst;
	if (!del || !*lst)
		return ;
	while (node)
	{
		put_content(node->content);
		del(node->content);
		temp = node;
		node = node->next;
		free(temp);
	}
	*lst = NULL;
}

int		free_all(t_vars *vars)
{
	mlx_destroy_image(vars->mlx, vars->frame.image);
	mlx_destroy_window(vars->mlx, vars->window);
	// free stuff
	ft_lstclear_ab(&vars->sprites, free);
	exit(1);
	return (1);
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
		free_all(vars);
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

// set to return dist to wall
void	cast_ray(t_ray *ray, t_vars *vars)
{	
	ray->sprites = 0;
	while (1)
	{
		if (ray->x && ft_point_len(ray->side_v) < ft_point_len(ray->side_h))
		{
			ray->side = (ray->x > 0) ? 'e' : 'w';
			ray->map_x += (ray->x >= 0) ? 1 : -1;
			if (vars->map[ray->map_y][ray->map_x] == '2')
			{
				t_list *node;
				node = vars->sprites;
				while (node)
				{
					if (((t_content *)(node->content))->x == ray->map_x &&
						((t_content *)(node->content))->y == ray->map_y)
						break;
					node = node->next;
				}
				if (node)
				{
					((t_content *)(node->content))->is_shown = 1;
					// ((t_content *)(node->content))->contact.x = ray->side_v.x;
					// ((t_content *)(node->content))->contact.y = ray->side_v.y;
					((t_content *)(node->content))->contact.x = ray->map_x - vars->player.pos.x + 0.5;
					((t_content *)(node->content))->contact.y = ray->map_y - vars->player.pos.y + 0.5;
				}
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
				t_list *node;
				node = vars->sprites;
				while (node)
				{
					if (((t_content *)(node->content))->x == ray->map_x &&
						((t_content *)(node->content))->y == ray->map_y)
						break;
					node = node->next;
				}
				if (node)
				{
					((t_content *)(node->content))->is_shown = 1;
					// ((t_content *)(node->content))->contact.x = ray->side_h.x;
					// ((t_content *)(node->content))->contact.y = ray->side_h.y;
					((t_content *)(node->content))->contact.x = ray->map_x - vars->player.pos.x + 0.5;
					((t_content *)(node->content))->contact.y = ray->map_y - vars->player.pos.y + 0.5;
				}
			}
			if (vars->map[ray->map_y][ray->map_x] == '1')
				break;
			ray->side_h = ft_point_add(ray->side_h, ray->delta_h);
		}
	}
}

double	calc_wall(t_ray *ray, t_vars *vars)
{
	double	cos_angle;
	double	dist_to_wall_perp;
	
	ray->wall.x = (ray->side == 'w' || ray->side == 'e') ? ray->side_v.x : ray->side_h.x;
	ray->wall.y = (ray->side == 'w' || ray->side == 'e') ? ray->side_v.y : ray->side_h.y;
	cos_angle = (vars->player.plane.x * ray->wall.x + vars->player.plane.y * ray->wall.y) /
				(ft_point_len(ray->wall) * ft_point_len(vars->player.plane));
	dist_to_wall_perp = sqrt(1 - cos_angle * cos_angle) * ft_point_len(ray->wall);
	ray->wall_height = vars->resolution.y / dist_to_wall_perp;
	ray->wall_start = -ray->wall_height / 2 + vars->resolution.y / 2;
	ray->wall_end = ray->wall_height / 2 + vars->resolution.y / 2;
	if (ray->wall_end >= vars->resolution.y)
		ray->wall_end = vars->resolution.y - 1;
	return (dist_to_wall_perp);
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
	draw_line(&vars->frame, vars->resolution.x - i - 1, 0, vars->resolution.x - i - 1, (ray->wall_start < 0) ? 0 : ray->wall_start, vars->texture.ceilling.argb);
	while (y < vars->resolution.y && y <= ray->wall_end)
	{
		if (ray->side == 'n')
			put_pixel(&vars->frame, vars->resolution.x - i - 1, y,
						calc_texture_color(ray, vars, vars->texture.north, y));
		else if (ray->side == 'e')
			put_pixel(&vars->frame, vars->resolution.x - i - 1, y,
						calc_texture_color(ray, vars, vars->texture.east, y));
		else if (ray->side == 'w')
			put_pixel(&vars->frame, vars->resolution.x - i - 1, y,
						calc_texture_color(ray, vars, vars->texture.west, y));
		else if (ray->side == 's')
			put_pixel(&vars->frame, vars->resolution.x - i - 1, y,
						calc_texture_color(ray, vars, vars->texture.south, y));
		y++;
	}
	draw_line(&vars->frame, vars->resolution.x - i - 1, vars->resolution.y - 1, vars->resolution.x - i - 1, ray->wall_end, vars->texture.floor.argb);
}

void	set_shown_to_zero(t_list **list)
{
	t_list *node;
	
	node = *list;
	if (!list)
		return ;
	while (node)
	{
		((t_content *)(node->content))->is_shown = 0;
		node = node->next;
	}
}

void	draw_sprites(t_vars *vars)
{
	
	// sort sprites - farthest first
	// node = vars->sprites;
	// while (node)
	// {
	// 	if (((t_content *)(node->content))->is_shown == 1)
	// 	{
			
	// 	}
	// }
	
	t_list *node;
	node = vars->sprites;
	ft_lst_put(vars->sprites, put_content);
	while (node)
	{
		if (!((t_content *)(node->content))->is_shown)
		{
			node = node->next;
			continue;
		}
		double determinant = 1.0 / (vars->player.plane.x * vars->player.direction.y -
									vars->player.direction.x * vars->player.plane.y);
		
		t_point transform;
		transform.x = determinant * (vars->player.direction.y * ((t_content *)(node->content))->contact.x -
									vars->player.direction.x * ((t_content *)(node->content))->contact.y);
		transform.y = determinant * (-vars->player.plane.y * ((t_content *)(node->content))->contact.x +
									vars->player.plane.x * ((t_content *)(node->content))->contact.y);
									
		int		sprite_screen_x = (int)((vars->resolution.x / 2) * (1 + transform.x / transform.y));
		
		int		sprite_height  = ft_abs((int)(vars->resolution.y / transform.y));
		int		sprite_start_y = -sprite_height / 2 + vars->resolution.y / 2;
		int		sprite_end_y   = sprite_height / 2 + vars->resolution.y / 2;
		if (sprite_start_y < 0)
			sprite_start_y = 0;
		if (sprite_end_y >= vars->resolution.y)
			sprite_end_y = vars->resolution.y - 1;
		
		int		sprite_width   = ft_abs((int)(vars->resolution.y / transform.y));
		int		sprite_start_x = -sprite_width / 2 + sprite_screen_x;
		int		sprite_end_x   = sprite_width / 2 + sprite_screen_x;
		if (sprite_start_x < 0)
			sprite_start_x = 0;
		if (sprite_end_x >= vars->resolution.x)
			sprite_end_x = vars->resolution.x - 1;
		
		int		stripe = sprite_start_x - 1;
		while (++stripe < sprite_end_x)
		{
			int tex_x = (int)(256 * (stripe - (-sprite_width / 2 + sprite_screen_x)) *
								512 / sprite_width) / 256;  // hard coded texture_width
			
			if (transform.y > 0 && transform.y < vars->rays[stripe]) // && transform.y < zbuffer[stripe]
			{
				int		y = sprite_start_y - 1;
				while (++y < sprite_end_y)
				{
					int d = y * 256 - vars->resolution.y * 128 + sprite_height * 128;
					int tex_y = ((d * 512) / sprite_height) / 256;
					unsigned int color = get_pixel(&vars->texture.sprite, tex_x, tex_y);
					if (color != 0x00B59696)
						put_pixel(&vars->frame, vars->resolution.x - stripe, y, color);
				}
			}
		}
		node = node->next;
	}
}

void	cast_rays(t_vars *vars)
{
	t_ray		ray;
	int			i;

	i = -1;
	vars->rays = malloc(sizeof(double) * vars->resolution.x);
	set_shown_to_zero(&vars->sprites);
	while (++i < vars->resolution.x)
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
		vars->rays[i] = calc_wall(&ray, vars);
		draw_wall(&ray, vars, i);
	}
}


int		render_next_frame(t_vars *vars)
{
	cast_rays(vars);
	draw_sprites(vars);
	free(vars->rays);
	mlx_put_image_to_window(vars->mlx, vars->window, vars->frame.image, 0, 0);
	return (1);
}

t_vars	*default_vars(void)
{
	t_vars	*vars;

	if (!(vars = malloc(sizeof(t_vars))))
		handle_error("Error: malloc couldn't allocate space\n");
	vars->y = 0;
	vars->player.speed = 0.15;
	vars->player.turn_speed = PI / 30;
	vars->mlx = mlx_init();
	vars->sprites = NULL;
	vars->rays = NULL;
	return (vars);
}

void	handle_error(const char *error)
{
	ft_putstr_fd((char *)error, STD_IN);
	exit(0);  // EXIT_FAILURE
}

t_image	new_frame(t_vars *vars)
{
	t_image frame;
	
	frame.image = 
	frame.image = mlx_new_image(vars->mlx, vars->resolution.x,
											vars->resolution.y);
	frame.address = mlx_get_data_addr(frame.image, &frame.bits_per_pixel,
													&frame.len,
													&frame.endian);
	return (frame);
}

int		main(int argc, char **argv)
{
	t_vars	*vars;

	if (argc == 2)
	{
		if (ft_strncmp(argv[1], "--save", 6) == 0)
		{
			// render and save image in bmp format.
		}
		else
		{
			vars = default_vars();
			read_map(argv[1], vars);

			vars->frame = new_frame(vars);
			vars->window = mlx_new_window(vars->mlx, vars->resolution.x, 
											vars->resolution.y, "cubcraft");
			mlx_hook(vars->window, 17, 1L << 17, free_all, vars);
			mlx_hook(vars->window, 2, 1L << 0, key_pressed, vars);
			mlx_loop_hook(vars->mlx, render_next_frame, vars);
			mlx_loop(vars->mlx);
		}
	}
	return (0);
}
