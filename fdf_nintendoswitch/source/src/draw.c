/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 17:56:08 by calamber          #+#    #+#             */
/*   Updated: 2019/09/07 05:36:24 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static int	ft_abs(int n)
{
	return ((n > 0) ? n : (n * -1));
}

void		dda(t_vox *start, t_vox *end, int color)
{
	int	dx;
	int	dy;
	int	steps;
	int	i;

	dx = (int)end->x - (int)start->x;
	dy = (int)end->y - (int)start->y;
	steps = ft_abs(dx) > ft_abs(dy) ? ft_abs(dx) : ft_abs(dy);
	i = 0;
	while (i <= steps)
	{
		if (i > 0 && i < steps)
			image_set_pixel(g_stuff.image, start->x, start->y, color);
			//mlx_pixel_put(s, s->win, start->x, start->y, color);
		start->x += dx / (float)steps;
		start->y += dy / (float)steps;
		i++;
	}
}

static void	mlx_draw_line(t_vox *a, t_vox *b)
{
	t_vox t_a;
	t_vox t_b;

	t_a = mlx_project(*a);
	t_b = mlx_project(*b);
	dda(&t_a, &t_b, (a->z != b->z) ? 14358738 - ((fmax(a->z, b->z) - fmin(a->z, b->z)) * 150) : a->c);
}

static void	draw_point(t_vox *v, int x, int y, t_mlx_stuff *stuff)
{
	t_vox *b;

	if ((int)v->x + 1 < (stuff->map.col))
	{
		b = &(stuff->map.v)[stuff->map.col * y + x + 1];
		mlx_draw_line(v, b);
	}
	if ((int)v->y - 1 >= 0)
	{
		b = &(stuff->map.v)[stuff->map.col * (y - 1) + x];
		mlx_draw_line(v, b);
	}
}

void		update_window(void)
{
	int		y;
	int		x;
	t_vox	*v;

	y = 0;
	clear_image(g_stuff.image);
	while (g_stuff.map.v && y < g_stuff.map.rows)
	{
		x = 0;
		while (g_stuff.map.v && x < g_stuff.map.col)
		{
			v = &g_stuff.map.v[g_stuff.map.col * y + x];
			draw_point(v, x, y, &g_stuff);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(g_stuff.s.mlx, g_stuff.s.win, g_stuff.image->image, 0, 0);
}
