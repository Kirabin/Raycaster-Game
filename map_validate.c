/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 13:25:18 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/11 17:42:48 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int				check_cub_extension(char *line)
{
	int i;

	i = 0;
	while (line[i])
		i++;
	if (i >= 4)
		return (ft_strncmp(line + i - 4, ".cub", 4) == 0);
	return (0);
}

static void		check_symbols(t_vars *vars)
{
	int		i;
	int		j;

	i = 0;
	while (i < vars->y)
	{
		j = 0;
		while (j < vars->x[i])
		{
			if (!ft_strchr(" 012NEWS", vars->map[i][j]))
				handle_error("Error: unknown symbols\n");
			j++;
		}
		i++;
	}
}

void			flood_fill(t_vars *vars, char **flood, int x, int y)
{
	if (x >= vars->x[y] || x < 0 || y < 0 || y >= vars->y ||
		vars->map[y][x] == ' ')
		handle_error("Error: map is not surrounded by walls\n");
	if (vars->map[y][x] != '1' && flood[y][x] == '0')
	{
		flood[y][x] = '1';
		flood_fill(vars, flood, x, y - 1);
		flood_fill(vars, flood, x + 1, y);
		flood_fill(vars, flood, x, y + 1);
		flood_fill(vars, flood, x - 1, y);
	}
	return ;
}

void			is_surrounded(t_vars *vars)
{
	char	**flood;
	int		i;

	if (!(flood = malloc(sizeof(char *) * vars->y)))
		handle_error("Error: malloc couldn't allocate space\n");
	i = 0;
	while (i < vars->y)
	{
		if (!(flood[i] = ft_calloc_c(vars->x[i], sizeof(char) + 1, '0')))
			handle_error("Error: malloc couldn't allocate space\n");
		flood[i][vars->x[i]] = 0;
		i++;
	}
	flood_fill(vars, flood, vars->player.pos.x, vars->player.pos.y);
	i = 0;
	while (i < vars->y)
		free(flood[i++]);
	free(flood);
}

int				map_validate(t_vars *vars)
{
	check_symbols(vars);
	is_surrounded(vars);
	return (1);
}
