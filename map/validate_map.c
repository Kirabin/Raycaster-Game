/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 13:25:18 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/05 10:52:53 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int		check_cub_extension(char *line)
{
	int i;

	i = 0;
	while (line[i])
		i++;
	if (i >= 4)
		return (ft_strncmp(line + i - 4, ".cub", 4) == 0);
	return (0);
}

int		validate_map(t_vars *vars)
{
	return (1);
}
