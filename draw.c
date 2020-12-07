/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 17:30:10 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/07 19:49:13 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	put_pixel(t_image *image, int x, int y, int color)
{
	char	*dst;
	
	dst = image->address + (y * image->len + x * (image->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int		get_pixel(t_image *image, int x, int y)
{
	char	*dst;
	
	dst = image->address + (y * image->len + x * (image->bits_per_pixel / 8));
	return (*(unsigned int *)dst);
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
	t_point		delta;
	t_point		pixel;
	float		ratio;
	int			x;
	int			y;

	delta.x = x2 - x1;
	delta.y = y2 - y1;
	if (ft_abs(delta.y) < ft_abs(delta.x))
	{
		x = -1;
		while (++x <= ft_abs(delta.x))
		{
			ratio = x * 1.0 / ft_abs(delta.x);
			pixel.x = x1 + x * (delta.x > 0 ? 1 : -1);
			pixel.y = y1 + ratio * ft_abs(delta.y) * (delta.y > 0 ? 1 : -1);
			put_pixel(frame, (int)pixel.x, (int)pixel.y, color);
		}
	}
	else
	{
		y = -1;
		while (++y <= ft_abs(delta.y))
		{
			ratio = y * 1.0 / ft_abs(delta.y);
			pixel.x = x1 + ratio * ft_abs(delta.x) * (delta.x > 0 ? 1 : -1);
			pixel.y = y1 + y * (delta.y > 0 ? 1 : -1);
			put_pixel(frame, (int)pixel.x, (int)pixel.y, color);
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
	
	i = ft_min(x1, x2) - 1;
	while (++i <= ft_max(x1, x2))
	{
		j = ft_min(y1, y2) - 1;
		while (++j <= ft_max(y1, y2))
			put_pixel(frame, i, j, color);
	}
}
