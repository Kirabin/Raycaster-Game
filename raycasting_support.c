/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_support.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 18:03:43 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/14 10:22:29 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void		check_for_sprite(t_ray *ray, t_vars *vars)
{
	t_list *node;

	if (vars->map[ray->map_y][ray->map_x] == '2')
	{
		node = vars->sprites;
		while (node)
		{
			if (((t_content *)(node->content))->x == ray->map_x &&
				((t_content *)(node->content))->y == ray->map_y)
				break ;
			node = node->next;
		}
		if (node)
		{
			((t_content *)(node->content))->is_shown = 1;
			((t_content *)(node->content))->contact.x = ray->map_x -
					vars->player.pos.x + 0.5;
			((t_content *)(node->content))->contact.y = ray->map_y -
					vars->player.pos.y + 0.5;
		}
	}
}

static int	cast_ray_x(t_ray *ray, t_vars *vars)
{
	ray->side = (ray->x > 0) ? 'e' : 'w';
	ray->map_x += (ray->x >= 0) ? 1 : -1;
	check_for_sprite(ray, vars);
	if (vars->map[ray->map_y][ray->map_x] == '1')
		return (1);
	ray->side_v = ft_point_add(ray->side_v, ray->delta_v);
	return (0);
}

void		cast_ray(t_ray *ray, t_vars *vars)
{
	while (1)
	{
		if (ray->x && ft_point_len(ray->side_v) < ft_point_len(ray->side_h))
		{
			if (cast_ray_x(ray, vars))
				break ;
		}
		else if (ray->y)
		{
			ray->side = (ray->y > 0) ? 's' : 'n';
			ray->map_y += (ray->y >= 0) ? 1 : -1;
			check_for_sprite(ray, vars);
			if (vars->map[ray->map_y][ray->map_x] == '1')
				break ;
			ray->side_h = ft_point_add(ray->side_h, ray->delta_h);
		}
		else if (ray->x)
		{
			if (cast_ray_x(ray, vars))
				break ;
		}
	}
}
