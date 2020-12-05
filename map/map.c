/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 10:21:52 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/05 11:19:58 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int		read_map(char *file, t_vars *vars)
{
	char		*line;
	int			flag;
	int			result;
	int			fd;
	int			i;
	int			j;
	char		*map_char;

	i = 0;
	flag = 0;
	if (!check_cub_extension(file))
	{
		write(1, "Error: map extension is not .cub\n", 33);
		// free
		exit(0);
	}
	if ((fd = open(file, O_RDONLY)) == -1)
	{
		write(1, "Error: couldn't open map file\n", 30);
		// free
		exit(0);
	}
	while ((result = get_next_line(fd, &line)) != 0)
	{
		if (result == -1)
		{
			// free(map);
			return (0);
		}
		if (!flag && parce_map_element(vars, line))
			continue;
		else
		{
			flag = 1;
			vars->y++;
		}
		free(line);
	}
	free(line);
	if (!(vars->x = malloc(sizeof(int) * vars->y)))
		return (0);
	if (!(vars->map = malloc(sizeof(char*) * vars->y)))
	{
		free(vars->x);
		return (0);
	}
	close(fd);
	fd = open(file, O_RDONLY);
	flag = 0;
	while ((result = get_next_line(fd, &line)) != 0)
	{
		if (result == -1)
		{
			// free(map);
			return (0);
		}
		if (!flag && parce_map_element(vars, line))
			continue;
		else
		{
			flag = 1;
			vars->x[i] = ft_strlen(line);
			if (!(vars->map[i] = ft_strdup(line)))
			{
				free(vars->x);
				// free allocated vars->field;
				return (0);
			}
			j = 0;
			while (j < vars->x[i])  // not effectitve
			{
				if (line[j] == 'N')
				{
					vars->player.direction.x = 0;
					vars->player.direction.y = -1;
					vars->player.pos.x = j + 0.5;
					vars->player.pos.y = i + 0.5;
					vars->player.plane.x = -0.66;
					vars->player.plane.y = 0;
				}
				else if (line[j] == 'E')
				{
					vars->player.direction.x = 1;
					vars->player.direction.y = 0;
					vars->player.pos.x = j + 0.5;
					vars->player.pos.y = i + 0.5;
					vars->player.plane.x = 0;
					vars->player.plane.y = -0.66;
				}
				else if (line[j] == 'W')
				{
					vars->player.direction.x = -1;
					vars->player.direction.y = 0;
					vars->player.pos.x = j + 0.5;
					vars->player.pos.y = i + 0.5;
					vars->player.plane.x = 0;
					vars->player.plane.y = 0.66;
				}
				else if (line[j] == 'S')
				{
					vars->player.direction.x = 0;
					vars->player.direction.y = 1;
					vars->player.pos.x = j + 0.5;
					vars->player.pos.y = i + 0.5;
					vars->player.plane.x = 0.66;
					vars->player.plane.y = 0;
				}
				j++;
			}
			i++;
		}
		free(line);
	}
	free(line);
	close(fd);
	return (1);  //free_map()
}
