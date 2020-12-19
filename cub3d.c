/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 09:54:39 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/19 16:10:43 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int		render_next_frame(t_vars *vars)
{
	cast_rays(vars);
	draw_sprites(vars);
	free(vars->rays);
	mlx_put_image_to_window(vars->mlx, vars->window,
							vars->frame.image, 0, 0);
	return (1);
}

t_vars			*default_vars(void)
{
	t_vars	*vars;

	if (!(vars = malloc(sizeof(t_vars))))
		handle_error("Error: malloc couldn't allocate space\n");
	vars->y = 0;
	vars->player.speed = 0.15;
	vars->player.is_set = false;
	vars->player.turn_speed = PI / 30;
	vars->mlx = mlx_init();
	vars->sprites = NULL;
	vars->rays = NULL;
	vars->save = false;
	return (vars);
}

void			handle_error(const char *error)
{
	ft_putstr_fd("Error\n", STD_ERROR);
	ft_putstr_fd((char *)error, STD_ERROR);
	exit(0);
}

static t_image	new_frame(t_vars *vars)
{
	t_image frame;

	frame.image = mlx_new_image(vars->mlx, vars->resolution.x,
											vars->resolution.y);
	frame.address = mlx_get_data_addr(frame.image, &frame.bits_per_pixel,
													&frame.len,
													&frame.endian);
	frame.width = vars->resolution.x;
	frame.height = vars->resolution.y;
	return (frame);
}

int				main(int argc, char **argv)
{
	t_vars	*vars;

	if (argc == 2 || argc == 3)
	{
		vars = default_vars();
		read_map(argv[1], vars);
		vars->frame = new_frame(vars);
		if (argc == 3)
		{
			if (ft_strncmp(argv[2], "--save", 6) == 0)
			{
				vars->save = true;
				render_frame_for_bmp(vars);
				free_all(vars);
			}
			return (0);
		}
		vars->window = mlx_new_window(vars->mlx, vars->resolution.x,
										vars->resolution.y, "cubcraft");
		mlx_hook(vars->window, 17, 1L << 17, free_all, vars);
		mlx_hook(vars->window, 2, 1L << 0, key_pressed, vars);
		mlx_loop_hook(vars->mlx, render_next_frame, vars);
		mlx_loop(vars->mlx);
	}
	return (0);
}
