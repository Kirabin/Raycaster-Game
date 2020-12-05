/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 13:20:44 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/05 10:27:26 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int			argb_color(int a, int r, int g, int b)
{
	return(a << 24 | r << 16 | g << 8 | b);
}

t_color		parce_color(const char *line)
{
	int		r;
	int		g;
	int		b;
	t_color	color;

	line = ft_strskip_char(line, ' ');
	color.r = ft_atoi(line);
	line = ft_strchr(line, ',') + 1;
	color.g = ft_atoi(line);
	line = ft_strchr(line, ',') + 1;
	color.b = ft_atoi(line);
	color.argb = argb_color(0, color.r, color.g, color.b);
	return (color);
}


int			dim_color(t_color color, int value)
{
	t_color new_color;
	
	new_color.r = (value > color.r) ? 0 : color.r - value;
	new_color.g = (value > color.g) ? 0 : color.g - value;
	new_color.b = (value > color.b) ? 0 : color.b - value;
	new_color.argb = argb_color(0, new_color.r, new_color.g, new_color.b);
	
	return (new_color.argb);
}