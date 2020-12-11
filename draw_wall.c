/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_wall.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 17:36:43 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/11 17:37:57 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void		draw_ceiling(t_ray *ray, t_vars *vars, int i)
{
	t_point		start;
	t_point		end;

	start.x = vars->resolution.x - i - 1;
	start.y = 0;
	end.x = vars->resolution.x - i - 1;
	end.y = (ray->wall_start < 0) ? 0 : ray->wall_start;
	draw_line(&vars->frame, start, end, vars->texture.ceilling.argb);
}

static void		draw_floor(t_ray *ray, t_vars *vars, int i)
{
	t_point		start;
	t_point		end;

	start.x = vars->resolution.x - i - 1;
	start.y = vars->resolution.y - 1;
	end.x = vars->resolution.x - i - 1;
	end.y = ray->wall_end;
	draw_line(&vars->frame, start, end, vars->texture.floor.argb);
}

static int		calc_texture_color(t_ray *ray, t_vars *vars, t_image texture,
									int y)
{
	int texture_y;
	int texture_x;

	texture_y = (y * 1.0 - ray->wall_start) / ray->wall_height * texture.height;
	if (ray->side == 'w' || ray->side == 'e')
		texture_x = (vars->player.pos.y + ray->wall.y - (int)(ray->wall.y +
					vars->player.pos.y)) * texture.width;
	else
		texture_x = (vars->player.pos.x + ray->wall.x - (int)(ray->wall.x +
					vars->player.pos.x)) * texture.width;
	return (get_pixel(&texture, texture_x, texture_y));
}

void			draw_wall(t_ray *ray, t_vars *vars, int i)
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
