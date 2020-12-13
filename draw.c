/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 17:30:10 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/13 15:27:08 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void			put_pixel(t_image *image, int x, int y, int color)
{
	char	*dst;

	dst = image->address + (y * image->len + x * (image->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

unsigned int	get_pixel(t_image *image, int x, int y)
{
	char	*dst;

	dst = image->address + (y * image->len + x * (image->bits_per_pixel / 8));
	return (*(unsigned int *)dst);
}

void			draw_line(t_image *frame, t_point p1, t_point p2, int color)
{
	t_point		pixel;
	float		ratio;
	int			x;
	int			y;

	x = -1;
	y = -1;
	if (ft_abs(p2.y - p1.y) < ft_abs(p2.x - p1.x))
		while (++x <= ft_abs(p2.x - p1.x))
		{
			ratio = x * 1.0 / ft_abs(p2.x - p1.x);
			pixel.x = p1.x + x * (p2.x - p1.x > 0 ? 1 : -1);
			pixel.y = p1.y + ratio * ft_abs(p2.y - p1.y) *
						(p2.y - p1.y > 0 ? 1 : -1);
			put_pixel(frame, (int)pixel.x, (int)pixel.y, color);
		}
	else
		while (++y <= ft_abs(p2.y - p1.y))
		{
			ratio = y * 1.0 / ft_abs(p2.y - p1.y);
			pixel.x = p1.x + ratio * ft_abs(p2.x - p1.x) *
						(p2.x - p1.x > 0 ? 1 : -1);
			pixel.y = p1.y + y * (p2.y - p1.y > 0 ? 1 : -1);
			put_pixel(frame, (int)pixel.x, (int)pixel.y, color);
		}
}
