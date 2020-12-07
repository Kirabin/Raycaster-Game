/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 10:21:52 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/07 19:01:56 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	check_line_for_player_posision(t_vars *vars, char *line, int i)
{
	int		j;

	j = -1;
	while (++j < vars->x[i])
	{
		if (line[j] == 'N')
		{
			vars->player.direction = ft_point_new(0, -1);
			vars->player.pos = ft_point_new(j + 0.5, i + 0.5);
			vars->player.plane = ft_point_new(-0.66, 0);
		}
		else if (line[j] == 'E')
		{
			vars->player.direction = ft_point_new(1, 0);
			vars->player.pos = ft_point_new(j + 0.5, i + 0.5);
			vars->player.plane = ft_point_new(0, -0.66);
		}
		else if (line[j] == 'W')
		{
			vars->player.direction = ft_point_new(-1, 0);
			vars->player.pos = ft_point_new(j + 0.5, i + 0.5);
			vars->player.plane = ft_point_new(0, 0.66);
		}
		else if (line[j] == 'S')
		{
			vars->player.direction = ft_point_new(0, 1);
			vars->player.pos = ft_point_new(j + 0.5, i + 0.5);
			vars->player.plane = ft_point_new(0.66, 0);
		}
	}
}

int			count_map_rows_in_file(char *file)
{
	int		i;
	int		fd;
	int		result;
	char	*line;

	i = 0;
	if ((fd = open(file, O_RDONLY)) == -1)
		handle_error("Error: couldn't open map file\n");
	while ((result = get_next_line(fd, &line)) != 0)
	{
		if (result == -1)
			handle_error("Error: get_next_line failed to read a file\n");
		if (!i && is_map_element(line))
			continue;
		i++;
		free(line);
	}
	free(line);
	close(fd);
	return (i);
}

void		parce_map_file(char *file, t_vars *vars)
{
	int		fd;
	int		i;
	int		result;
	char	*line;

	if ((fd = open(file, O_RDONLY)) == -1)
		handle_error("Error: couldn't open map file\n");
	i = 0;
	while ((result = get_next_line(fd, &line)) != 0)
	{
		if (result == -1)
			handle_error("Error: get_next_line failed to read a file\n");
		if (!i && parce_map_element(vars, line))
			continue;
		vars->x[i] = ft_strlen(line);
		if (!(vars->map[i] = ft_strdup(line)))
			handle_error("Error: malloc couldn't allocate space\n");
		check_line_for_player_posision(vars, line, i);
		i++;
		free(line);
	}
	free(line);
	close(fd);
}

void		read_map(char *file, t_vars *vars)
{
	if (!check_cub_extension(file))
		handle_error("Error: map extension is not .cub\n");
	vars->y = count_map_rows_in_file(file);
	if (!(vars->x = malloc(sizeof(int) * vars->y)))
		handle_error("Error: malloc couldn't allocate space\n");
	if (!(vars->map = malloc(sizeof(char*) * vars->y)))
		handle_error("Error: malloc couldn't allocate space\n");
	parce_map_file(file, vars);
	map_validate(vars);
}
