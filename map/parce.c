/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parce.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/05 10:45:28 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/05 10:45:48 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	parce_resolution(t_vars *vars, const char *line)
{
	int		max_x;
	int		max_y;

	line = ft_strskip_char(line, ' ');
	vars->resolution.x = ft_atoi(line);
	line = ft_strskip_char(ft_strchr(line, ' '), ' ');
	vars->resolution.y = ft_atoi(line);
}

void	new_texture(t_vars *vars, t_image *texture, const char *line)
{
	texture->image = mlx_xpm_file_to_image(
						vars->mlx, (char *)line,
						&texture->width,
						&texture->height);
	texture->address = mlx_get_data_addr(
						texture->image,
						&texture->bits_per_pixel, &texture->len,
						&texture->endian);
}

int		parce_map_element(t_vars *vars, char *line)
{
	if (*line == 'R')
		parce_resolution(vars, line + 1);
	else if (*line == 'F')
		vars->texture.floor = parce_color(line + 1);
	else if (*line == 'C')
		vars->texture.ceilling = parce_color(line + 1);
	else if (*line == 'S' && *(line + 1) != 'O')
		new_texture(vars, &vars->texture.sprite, ft_strskip_char(line + 1, ' '));
	else if (ft_strncmp(line, "NO", 2) == 0)
		new_texture(vars, &vars->texture.north, ft_strskip_char(line + 2, ' '));
	else if (ft_strncmp(line, "EA", 2) == 0)
		new_texture(vars, &vars->texture.east, ft_strskip_char(line + 2, ' '));
	else if (ft_strncmp(line, "SO", 2) == 0)
		new_texture(vars, &vars->texture.south, ft_strskip_char(line + 2, ' '));
	else if (ft_strncmp(line, "WE", 2) == 0)
		new_texture(vars, &vars->texture.west, ft_strskip_char(line + 2, ' '));
	else if (*line == '\n' || *line == '\0')
		;
	else
		return (0);
	return (1);
}
