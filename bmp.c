/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 18:09:26 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/14 12:17:21 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void					render_frame_for_bmp(t_vars *vars)
{
	cast_rays(vars);
	draw_sprites(vars);
	free(vars->rays);
	create_bmp(vars->frame);
}

/*
**	FILE_HEADER
**	0,0,      -signature
**	0,0,0,0,  -image file size in bytes
**	0,0,0,0,  -reserved
**	0,0,0,0,  -start of pixel array
*/

static unsigned char	*get_bitmap_file_header(t_image frame)
{
	static unsigned char	file_header[14];
	unsigned int			file_size;

	file_size = frame.height * frame.width + 14 + 40;
	file_header[0] = (unsigned char)('B');
	file_header[1] = (unsigned char)('M');
	file_header[2] = (unsigned char)(file_size);
	file_header[3] = (unsigned char)(file_size >> 8);
	file_header[4] = (unsigned char)(file_size >> 16);
	file_header[5] = (unsigned char)(file_size >> 24);
	file_header[10] = (unsigned char)(14 + 40);
	return (file_header);
}

/*
**	INFO_HEADER
**	0,0,0,0,  —header size
**	0,0,0,0,  —image width
**	0,0,0,0,  —image height
**	0,0,      —number of color planes
**	0,0,      —bits per pixel
**	0,0,0,0,  —compression
**	0,0,0,0,  —image size
**	0,0,0,0,  —horizontal resolution
**	0,0,0,0,  —vertical resolution
**	0,0,0,0,  —colors in color table
**	0,0,0,0,  —important color count
*/

static unsigned char	*get_bitmap_info_header(t_image frame)
{
	static unsigned char info_header[40];

	info_header[0] = (unsigned char)(40);
	info_header[4] = (unsigned char)(frame.width);
	info_header[5] = (unsigned char)(frame.width >> 8);
	info_header[6] = (unsigned char)(frame.width >> 16);
	info_header[7] = (unsigned char)(frame.width >> 24);
	info_header[8] = (unsigned char)(frame.height);
	info_header[9] = (unsigned char)(frame.height >> 8);
	info_header[10] = (unsigned char)(frame.height >> 16);
	info_header[11] = (unsigned char)(frame.height >> 24);
	info_header[12] = (unsigned char)(1);
	info_header[14] = (unsigned char)(frame.bits_per_pixel);
	return (info_header);
}

void					create_bmp(t_image frame)
{
	int				i;
	int				j;
	int				fd;
	unsigned int	pixel;

	if ((fd = open("first_frame.bmp", O_CREAT | O_WRONLY, 0644)) == -1)
		handle_error("Error: couldn't create .bpm file");
	write(fd, get_bitmap_file_header(frame), 14);
	write(fd, get_bitmap_info_header(frame), 40);
	i = frame.height;
	while (--i > 0)
	{
		j = -1;
		while (++j < frame.width)
		{
			pixel = get_pixel(&frame, j, i);
			ft_putuc_fd((unsigned char)pixel, fd);
			ft_putuc_fd((unsigned char)(pixel >> 8), fd);
			ft_putuc_fd((unsigned char)(pixel >> 16), fd);
			ft_putuc_fd((unsigned char)(pixel >> 24), fd);
		}
	}
	close(fd);
}
