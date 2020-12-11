/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprites.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmilan <dmilan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 17:22:20 by dmilan            #+#    #+#             */
/*   Updated: 2020/12/11 17:35:39 by dmilan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int					compare_sprites(t_list *a, t_list *b)
{
	if (ft_point_len(((t_content *)(a->content))->contact) <
		ft_point_len(((t_content *)(b->content))->contact))
		return (1);
	return (0);
}

static t_point		calc_transform(t_vars *vars, t_content *content)
{
	t_point			transform;
	double			determinant;

	determinant = 1.0 / (vars->player.plane.x * vars->player.direction.y -
						vars->player.direction.x * vars->player.plane.y);
	transform.x = determinant * (vars->player.direction.y * content->contact.x -
					vars->player.direction.x * content->contact.y);
	transform.y = determinant * (-vars->player.plane.y * content->contact.x +
					vars->player.plane.x * content->contact.y);
	return (transform);
}

static void			put_sprite_pixel(t_vars *vars, t_content *content,
										int x, int y)
{
	int				d;
	unsigned int	color;

	d = y * 256 - vars->resolution.y * 128 + content->height * 128;
	content->tex_x = (int)(256 * (x - (-content->width / 2 +
						content->screen_x)) * vars->texture.sprite.width /
						content->width) / 256;
	content->tex_y = ((d * vars->texture.sprite.height) / content->height)
						/ 256;
	color = get_pixel(&vars->texture.sprite, content->tex_x, content->tex_y);
	if (color != SPRITE_BG)
		put_pixel(&vars->frame, vars->resolution.x - x, y, color);
}

static void			calc_content_vars(t_vars *vars, t_content *content)
{
	content->transform = calc_transform(vars, content);
	content->screen_x = (vars->resolution.x / 2) *
							(1 + content->transform.x / content->transform.y);
	content->height = ft_abs((int)(vars->resolution.y / content->transform.y));
	content->start_y = -content->height / 2 + vars->resolution.y / 2;
	content->end_y = content->height / 2 + vars->resolution.y / 2;
	if (content->start_y < 0)
		content->start_y = 0;
	if (content->end_y >= vars->resolution.y)
		content->end_y = vars->resolution.y - 1;
	content->width = ft_abs((int)(vars->resolution.y / content->transform.y));
	content->start_x = -content->width / 2 + content->screen_x;
	content->end_x = content->width / 2 + content->screen_x;
	if (content->start_x < 0)
		content->start_x = 0;
	if (content->end_x >= vars->resolution.x)
		content->end_x = vars->resolution.x - 1;
}

void				draw_sprites(t_vars *vars)
{
	t_list		*node;
	t_content	*content;
	t_point		screen;

	ft_lst_bubble_sort(&vars->sprites, compare_sprites);
	node = vars->sprites;
	while (node)
	{
		content = (t_content *)(node->content);
		if (content->is_shown)
		{
			calc_content_vars(vars, content);
			screen.x = content->start_x - 1;
			while (++screen.x < content->end_x)
			{
				if (content->transform.y <= 0 ||
					content->transform.y >= vars->rays[(int)screen.x])
					continue;
				screen.y = content->start_y - 1;
				while (++screen.y < content->end_y)
					put_sprite_pixel(vars, content, screen.x, screen.y);
			}
		}
		node = node->next;
	}
}
