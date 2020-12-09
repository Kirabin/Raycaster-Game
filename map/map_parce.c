/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parce.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/05 10:45:28 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/08 15:23:35 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void		parce_resolution_line(t_vars *vars, const char *line)
{
	// int		max_x;
	// int		max_y;

	// mlx_get_screen_size(vars->mlx, &max_x, &max_y);
	// ft_printf("%d %d\n", max_x, max_y);
	line = ft_strskip_char(line, ' ');
	vars->resolution.x = ft_atoi(line);
	// if (vars->resolution.x > max_x)
	// 	vars->resolution.x = max_x;
	line = ft_strskip_char(ft_strchr(line, ' '), ' ');
	vars->resolution.y = ft_atoi(line);
	// if (vars->resolution.y > max_y)
	// 	vars->resolution.y = max_y;
}

void		new_texture(t_vars *vars, t_image *texture, const char *line)
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

int			parce_map_element(t_vars *vars, char *line)  //check for duplicates
{
	if (*line == 'R')
		parce_resolution_line(vars, line + 1);
	else if (*line == 'F')
		vars->texture.floor = parce_color_line(line + 1);
	else if (*line == 'C')
		vars->texture.ceilling = parce_color_line(line + 1);
	else if (*line == 'S' && *(line + 1) == ' ')
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

int			is_map_element(char *line)
{
	if (*line == 'R')
		;
	else if (*line == 'F')
		;
	else if (*line == 'C')
		;
	else if (*line == 'S' && *(line + 1) == ' ')
		;
	else if (ft_strncmp(line, "NO", 2) == 0)
		;
	else if (ft_strncmp(line, "EA", 2) == 0)
		;
	else if (ft_strncmp(line, "SO", 2) == 0)
		;
	else if (ft_strncmp(line, "WE", 2) == 0)
		;
	else if (*line == '\n' || *line == '\0')
		;
	else
		return (0);
	return (1);
}

t_color		parce_color_line(const char *line)
{
	t_color	color;

	line = ft_strskip_char(line, ' ');
	color.r = ft_atoi(line);
	line = ft_strchr(line, ',') + 1;
	color.g = ft_atoi(line);
	line = ft_strchr(line, ',') + 1;
	color.b = ft_atoi(line);
	color.argb = ft_color_argb(0, color.r, color.g, color.b);
	return (color);
}