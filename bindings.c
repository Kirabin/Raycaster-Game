/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bindings.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 13:59:12 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/13 13:57:25 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void		move(t_vars *vars, double angle)
{
	vars->player.pos.x += ft_point_rotate(vars->player.direction, angle).x
						* vars->player.speed;
	if (vars->map[(int)vars->player.pos.y][(int)vars->player.pos.x] == '1')
		vars->player.pos.x -= ft_point_rotate(vars->player.direction, angle).x
							* vars->player.speed;
	vars->player.pos.y += ft_point_rotate(vars->player.direction, angle).y
						* vars->player.speed;
	if (vars->map[(int)vars->player.pos.y][(int)vars->player.pos.x] == '1')
		vars->player.pos.y -= ft_point_rotate(vars->player.direction, angle).y
							* vars->player.speed;
}

static void		rotate(t_vars *vars, double angle)
{
	vars->player.direction = ft_point_rotate(vars->player.direction, angle);
	vars->player.plane = ft_point_rotate(vars->player.plane, angle);
}

int				key_pressed(int keycode, t_vars *vars)
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
	return (1);
}

int				free_all(t_vars *vars)
{
	int		i;

	i = 0;
	mlx_destroy_image(vars->mlx, vars->frame.image);
	if (vars->save == 0)
		mlx_destroy_window(vars->mlx, vars->window);
	while (i < vars->y)
		free(vars->map[i++]);
	free(vars->map);
	free(vars->x);
	free(vars);
	ft_lstclear(&vars->sprites, free);
	exit(0);
	return (1);
}
