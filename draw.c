/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 17:30:10 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/11 16:49:28 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	put_pixel(t_image *image, int x, int y, int color)
{
	char	*dst;

	dst = image->address + (y * image->len + x * (image->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int			get_pixel(t_image *image, int x, int y)
{
	char	*dst;

	dst = image->address + (y * image->len + x * (image->bits_per_pixel / 8));
	return (*(unsigned int *)dst);
}

void		draw_line(t_image *frame, t_point p1, t_point p2, int color)
{
	t_point		pixel;
	float		ratio;
	int			x;
	int			y;

	x = -1;
	y = -1;
	if (ft_abs(p2.y - p1.y) < ft_abs(p2.x - p1.x))
		while (++x <= ft_abs(p2.x - p1.x))
		{
			ratio = x * 1.0 / ft_abs(p2.x - p1.x);
			pixel.x = p1.x + x * (p2.x - p1.x > 0 ? 1 : -1);
			pixel.y = p1.y + ratio * ft_abs(p2.y - p1.y) *
						(p2.y - p1.y > 0 ? 1 : -1);
			put_pixel(frame, (int)pixel.x, (int)pixel.y, color);
		}
	else
		while (++y <= ft_abs(p2.y - p1.y))
		{
			ratio = y * 1.0 / ft_abs(p2.y - p1.y);
			pixel.x = p1.x + ratio * ft_abs(p2.x - p1.x) *
						(p2.x - p1.x > 0 ? 1 : -1);
			pixel.y = p1.y + y * (p2.y - p1.y > 0 ? 1 : -1);
			put_pixel(frame, (int)pixel.x, (int)pixel.y, color);
		}
}

void		draw_ceiling(t_ray *ray, t_vars *vars, int i)
{
	t_point		start;
	t_point		end;

	start.x = vars->resolution.x - i - 1;
	start.y = 0;
	end.x = vars->resolution.x - i - 1;
	end.y = (ray->wall_start < 0) ? 0 : ray->wall_start;
	draw_line(&vars->frame, start, end, vars->texture.ceilling.argb);
}

void		draw_floor(t_ray *ray, t_vars *vars, int i)
{
	t_point		start;
	t_point		end;

	start.x = vars->resolution.x - i - 1;
	start.y = vars->resolution.y - 1;
	end.x = vars->resolution.x - i - 1;
	end.y = ray->wall_end;
	draw_line(&vars->frame, start, end, vars->texture.floor.argb);
}

static int	calc_texture_color(t_ray *ray, t_vars *vars, t_image texture, int y)
{
	int sprite_y;
	int sprite_x;

	sprite_y = (y * 1.0 - ray->wall_start) / ray->wall_height * texture.height;
	if (ray->side == 'w' || ray->side == 'e')
		sprite_x = (vars->player.pos.y + ray->wall.y - (int)(ray->wall.y +
					vars->player.pos.y)) * texture.width;
	else
		sprite_x = (vars->player.pos.x + ray->wall.x - (int)(ray->wall.x +
					vars->player.pos.x)) * texture.width;
	return (get_pixel(&texture, sprite_x, sprite_y));
}

void		draw_wall(t_ray *ray, t_vars *vars, int i)
{
	int	y;

	y = (ray->wall_start < 0) ? 0 : ray->wall_start;
	draw_floor(ray, vars, i);
	while (y < vars->resolution.y && y < ray->wall_end)
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
	draw_ceiling(ray, vars, i);
}

int				compare_sprites(t_list *a, t_list *b)
{
	if (ft_point_len(((t_content *)(a->content))->contact) <
		ft_point_len(((t_content *)(b->content))->contact))
		return (1);
	return (0);
}

void			draw_sprites(t_vars *vars)
{
	t_list *node;

	ft_lst_bubble_sort(&vars->sprites, compare_sprites);
	node = vars->sprites;
	while (node)
	{
		if (!((t_content *)(node->content))->is_shown)
		{
			node = node->next;
			continue;
		}
		double determinant = 1.0 / (vars->player.plane.x * vars->player.direction.y - vars->player.direction.x * vars->player.plane.y); //static
		
		t_point transform;
		transform.x = determinant * (vars->player.direction.y * ((t_content *)(node->content))->contact.x - vars->player.direction.x * ((t_content *)(node->content))->contact.y);
		transform.y = determinant * (-vars->player.plane.y * ((t_content *)(node->content))->contact.x + vars->player.plane.x * ((t_content *)(node->content))->contact.y);
									
		int		sprite_screen_x = (int)((vars->resolution.x / 2) * (1 + transform.x / transform.y));

		int		sprite_height = ft_abs((int)(vars->resolution.y / transform.y));
		int		sprite_start_y = -sprite_height / 2 + vars->resolution.y / 2;
		int		sprite_end_y = sprite_height / 2 + vars->resolution.y / 2;
		if (sprite_start_y < 0)
			sprite_start_y = 0;
		if (sprite_end_y >= vars->resolution.y)
			sprite_end_y = vars->resolution.y - 1;

		int		sprite_width = ft_abs((int)(vars->resolution.y / transform.y));
		int		sprite_start_x = -sprite_width / 2 + sprite_screen_x;
		int		sprite_end_x = sprite_width / 2 + sprite_screen_x;
		if (sprite_start_x < 0)
			sprite_start_x = 0;
		if (sprite_end_x >= vars->resolution.x)
			sprite_end_x = vars->resolution.x - 1;
		int		stripe = sprite_start_x - 1;
		while (++stripe < sprite_end_x)
		{
			int tex_x = (int)(256 * (stripe - (-sprite_width / 2 + sprite_screen_x)) * 512 / sprite_width) / 256;  // hard coded texture_width
			if (transform.y > 0 && transform.y < vars->rays[stripe])
			{
				int		y = sprite_start_y - 1;
				while (++y < sprite_end_y)
				{
					int d;
					int tex_y;
					unsigned int color;
					d = y * 256 - vars->resolution.y * 128 +
								sprite_height * 128;
					tex_y = ((d * 512) / sprite_height) / 256;
					color = get_pixel(&vars->texture.sprite, tex_x, tex_y);
					if (color != 0x00B59696)
						put_pixel(&vars->frame, vars->resolution.x - stripe, y, color);
				}
			}
		}
		node = node->next;
	}
}