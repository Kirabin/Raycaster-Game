/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 17:30:10 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/02 12:20:06 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	put_pixel(t_image *image, int x, int y, int color)
{
	char	*dst;
	
	dst = image->address + (y * image->len + x * (image->bits_per_pixel / 8));  // why offset;
	*(unsigned int *)dst = color;
}

void	put_pixel_from_texture(t_image *frame, int x1, int y1, t_image *texture, int x2, int y2)
{
	char	*dst;
	char	*src;
	
	src = texture->address + (y2 * texture->len + x2 * (texture->bits_per_pixel / 8));
	dst = frame->address + (y1 * frame->len + x1 * (frame->bits_per_pixel / 8));
	*(unsigned int *)dst = *(unsigned int *)src;
}


void	draw_line(t_image *frame, int x1, int y1, int x2, int y2, int color)
{
	t_point	delta;
	float			ratio;
	int				x;
	int				y;

	ratio = 0;
	delta.x = x2 - x1;
	delta.y = y2 - y1;
	if (ft_absi(delta.y) < ft_absi(delta.x))
	{
		x = 0;
		while (x <= ft_absi(delta.x))
		{
			ratio = x * 1.0 / ft_absi(delta.x);
			put_pixel(frame, x1 + x * (delta.x > 0 ? 1 : -1), y1 + ratio * ft_absi(delta.y) * (delta.y > 0 ? 1 : -1), color);
			x++;
		}
	}
	else
	{
		y = 0;
		while (y <= ft_absi(delta.y))
		{
			ratio = y * 1.0 / ft_absi(delta.y);
			put_pixel(frame, x1 + ratio * ft_absi(delta.x) * (delta.x > 0 ? 1 : -1), y1 + y * (delta.y > 0 ? 1 : -1), color);
			y++;
		}
	}
}

void	draw_circle(t_image *frame, t_point center, int radius, int color)  // O(n^2)
{
	int		x;
	int		y;
	
	x = -radius;
	while (x <= radius)
	{
		y = -radius;
		while (y <= radius)
		{
			if (x * x + y * y <= radius * radius)
				put_pixel(frame, center.x + x, center.y + y, color);
			y++;
		}
		x++;
	}
}

void	draw_square(t_image *frame, int x, int y, int radius, int color)
{
	int		i;
	int		j;
	
	i = x;
	while (i <= radius + x)
	{
		j = y;
		while (j <= radius + y)
		{
			put_pixel(frame, i, j, color);
			j++;
		}
		i++;
	}
}

void	draw_rectangle(t_image *frame, int x1, int y1, int x2, int y2, int color)
{
	int		i;
	int		j;
	
	i = 1;
	while (i <= x2)
	{
		j = y1;
		while (j <= y2)
		{
			put_pixel(frame, i, j, color);
			j++;
		}
		i++;
	}
}

void	draw_line_gradient(t_image *frame, int x1, int y1, int x2, int y2, t_color color)
{
	t_point	delta;
	float			ratio;
	int				x;
	int				y;

	ratio = 0;
	delta.x = x2 - x1;
	delta.y = y2 - y1;
	if (ft_absi(delta.y) < ft_absi(delta.x))
	{
		x = 0;
		while (x <= ft_absi(delta.x))
		{
			ratio = x * 1.0 / ft_absi(delta.x);
			put_pixel(frame, x1 + x * (delta.x > 0 ? 1 : -1), y1 + ratio * ft_absi(delta.y) * (delta.y > 0 ? 1 : -1), dim_color(color, y / 3));
			x++;
		}
	}
	if (1)
	{
		y = 0;
		while (y <= ft_absi(delta.y))
		{
			ratio = y * 1.0 / ft_absi(delta.y);
			put_pixel(frame, x1 + ratio * ft_absi(delta.x) * (delta.x > 0 ? 1 : -1), y1 + y * (delta.y > 0 ? 1 : -1), dim_color(color, y / 3));
			y++;
		}
	}
}