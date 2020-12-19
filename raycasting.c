/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 13:55:56 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/19 16:32:01 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void		calc_delta(t_ray *ray)
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

static void		calc_side(t_ray *ray, t_vars *vars)
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

static double	calc_wall(t_ray *ray, t_vars *vars)
{
	double	cos_angle;
	double	dist_to_wall_perp;

	ray->wall.x = (ray->side == 'w' || ray->side == 'e') ?
					ray->side_v.x : ray->side_h.x;
	ray->wall.y = (ray->side == 'w' || ray->side == 'e') ?
					ray->side_v.y : ray->side_h.y;
	cos_angle = (vars->player.plane.x * ray->wall.x +
				vars->player.plane.y * ray->wall.y) /
				(ft_point_len(ray->wall) * ft_point_len(vars->player.plane));
	dist_to_wall_perp = sqrt(1 - cos_angle * cos_angle) *
				ft_point_len(ray->wall);
	ray->wall_height = ceil(vars->resolution.y * 1.0 / dist_to_wall_perp);
	ray->wall_start = -floor(ray->wall_height * 1.0 / 2) +
						floor(vars->resolution.y * 1.0 / 2);
	ray->wall_end = floor(ray->wall_height * 1.0 / 2) +
						floor(vars->resolution.y * 1.0 / 2);
	if (ray->wall_end >= vars->resolution.y)
		ray->wall_end = vars->resolution.y - 1;
	return (dist_to_wall_perp);
}

static void		set_shown_to_zero(t_list **list)
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

void			cast_rays(t_vars *vars)
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
		ray.x = vars->player.direction.x + vars->player.plane.x *
					(i * 2.0 / vars->resolution.x - 1);
		ray.y = vars->player.direction.y + vars->player.plane.y *
					(i * 2.0 / vars->resolution.x - 1);
		calc_delta(&ray);
		calc_side(&ray, vars);
		cast_ray(&ray, vars);
		vars->rays[i] = calc_wall(&ray, vars);
		draw_wall(&ray, vars, i);
	}
}
