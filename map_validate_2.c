/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validate_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 17:43:01 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/11 17:54:32 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

void		check_char_for_player_position(t_vars *vars, char c)
{
	if (c == 'N')
	{
		vars->player.direction = ft_point_new(0, -1);
		vars->player.plane = ft_point_new(-0.66, 0);
	}
	else if (c == 'E')
	{
		vars->player.direction = ft_point_new(1, 0);
		vars->player.plane = ft_point_new(0, -0.66);
	}
	else if (c == 'W')
	{
		vars->player.direction = ft_point_new(-1, 0);
		vars->player.plane = ft_point_new(0, 0.66);
	}
	else if (c == 'S')
	{
		vars->player.direction = ft_point_new(0, 1);
		vars->player.plane = ft_point_new(0.66, 0);
	}
}

void		check_line_for_player_position(t_vars *vars, char *line, int i)
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
			vars->player.pos = ft_point_new(j + 0.5, i + 0.5);
		}
		check_char_for_player_position(vars, line[j]);
	}
}

void		check_line_for_sprites(t_vars *vars, char *line, int i)
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
