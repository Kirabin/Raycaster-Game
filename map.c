/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 10:21:52 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/11 17:55:44 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void		put_content(void *content)
{
	t_content cont;

	cont = *(t_content *)content;
	ft_printf("x: %d, y: %d, is_shown: %d\n", cont.x, cont.y, cont.is_shown);
}

void		check_for_map_symbols(char *line)
{
	int i;

	i = 0;
	while (line[i])
	{
		if (!ft_strchr(" 012NEWS", line[i]))
			handle_error("Error: .cub syntax error");
		i++;
	}
}

int			count_map_rows_in_file(char *file, t_elements *elements)
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
		if (!i && is_map_element(line, elements))
			continue;
		check_for_map_symbols(line);
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
		check_line_for_player_position(vars, line, i);
		check_line_for_sprites(vars, line, i);
		i++;
		free(line);
	}
	free(line);
	close(fd);
}

void		read_map(char *file, t_vars *vars)
{
	t_elements elements;

	default_elements(&elements);
	if (!check_cub_extension(file))
		handle_error("Error: map extension is not .cub\n");
	vars->y = count_map_rows_in_file(file, &elements);
	check_elements(&elements);
	if (!(vars->x = malloc(sizeof(int) * vars->y)))
		handle_error("Error: malloc couldn't allocate space\n");
	if (!(vars->map = malloc(sizeof(char*) * vars->y)))
		handle_error("Error: malloc couldn't allocate space\n");
	parce_map_file(file, vars);
	map_validate(vars);
}
