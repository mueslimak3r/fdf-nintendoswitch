/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 17:47:37 by calamber          #+#    #+#             */
/*   Updated: 2019/09/07 05:35:04 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static int	fdf_key_hook(int key, t_mlxp *p)
{
	if (key == KEY_ESCAPE)
	{
		mlxdel();
		exit (0);
	}
	if (key == KEY_LEFT)// || key == KEY_A)
		g_rot_offset.y += 0.06;
	if (key == KEY_RIGHT)// || key == KEY_D)
		g_rot_offset.y -= 0.06;
	if (key == KEY_UP)// || key == KEY_W)
		g_rot_offset.x -= 0.06;
	if (key == KEY_DOWN)// || key == KEY_S)
		g_rot_offset.x += 0.06;
	if (key == KEY_F)// || key == KEY_S)
		g_stuff.map.tile *= 1.5;
	if (key == KEY_G)// || key == KEY_S)
		g_stuff.map.tile *= 0.5;
	update_window();
	mlx_string_put(p->mlx, p->win, 10, W_YSIZE - 30, WHITE, "ESC: close");
	return (0);
}

int			newwindow(t_mlxp *s)
{
	ft_memset(s, 0, sizeof(t_mlxp));
	if (!(s->mlx = mlx_init()) ||
		!(s->win = mlx_new_window(s->mlx, W_XSIZE, W_YSIZE, "fdf")))
	{
		if (s->win)
			mlx_destroy_window(s->mlx, s->win);
		return (errorfunc("mlx window make fail", 0));
	}
	return (1);
}

void		makehooks(t_mlxp *s)
{
	mlx_hook(s->win, 2, 5, fdf_key_hook, s);
	mlx_string_put(s->mlx, s->win, 10, W_YSIZE - 30, WHITE, "ESC: close");
	mlx_loop(s->mlx);
}
