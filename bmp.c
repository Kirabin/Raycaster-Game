/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 18:09:26 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/12 11:11:15 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void			render_frame_for_bmp(t_vars *vars)
{
	cast_rays(vars);
	draw_sprites(vars);
	free(vars->rays);
	create_bmp(vars);
}

void	create_bmp(t_vars *vars)
{
	int		fd;

	fd = open("first_frame.bmp", O_CREAT | O_WRONLY);
	
	
	close(fd);
}