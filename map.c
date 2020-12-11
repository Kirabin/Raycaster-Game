/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 10:21:52 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/11 14:31:27 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	check_line_for_player_posision(t_vars *vars, char *line, int i)
{
	int		j;

	j = -1;
	while (++j < vars->x[i])
	{
		if (ft_strchr("NEWS", line[j]))
		{
			if (vars->player.is_set == true)
				handle_error("Error: multiple player positions");
			vars->player.is_set = true;
		}
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

void		put_content(void *content)
{
	t_content cont;

	cont = *(t_content *)content;
	ft_printf("x: %d, y: %d, is_shown: %d\n", cont.x, cont.y, cont.is_shown);
}

static void	check_line_for_sprites(t_vars *vars, char *line, int i)
{
	int			j;
	t_content	*content;

	j = -1;
	while (++j < vars->x[i])
	{
		if (line[j] == '2')
		{
			if (!(content = malloc(sizeof(t_content))))
				handle_error("Error: malloc couldn't allocate space\n");
			content->x = j;
			content->y = i;
			content->is_shown = false;
			ft_lstadd_back(&vars->sprites, ft_lstnew(content));
		}
	}
}

static void		check_for_map_symbols(char *line)
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
		check_line_for_player_posision(vars, line, i);
		check_line_for_sprites(vars, line, i);
		i++;
		free(line);
	}
	// ft_lst_put(vars->sprites, put_content);
	free(line);
	close(fd);
}

void		default_elements(t_elements *elements)
{
	elements->resolution_is_set = false;
	elements->north_is_set = false;
	elements->south_is_set = false;
	elements->west_is_set = false;
	elements->east_is_set = false;
	elements->sprite_is_set = false;
	elements->floor_is_set = false;
	elements->ceiling_is_set = false;
}

void		check_elements(t_elements *elements)
{
	if (elements->resolution_is_set != 1)
		handle_error("Error: map resolution error");
	if (elements->north_is_set != 1)
		handle_error("Error: map north texture error");
	if (elements->south_is_set != 1)
		handle_error("Error: map south texture error");
	if (elements->west_is_set != 1)
		handle_error("Error: map west texture error");
	if (elements->east_is_set != 1)
		handle_error("Error: map east texture error");
	if (elements->sprite_is_set != 1)
		handle_error("Error: map sprite error");
	if (elements->floor_is_set != 1)
		handle_error("Error: map floor color error");
	if (elements->ceiling_is_set != 1)
		handle_error("Error: map ceiling color error");
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
