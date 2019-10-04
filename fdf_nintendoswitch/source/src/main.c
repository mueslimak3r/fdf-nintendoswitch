/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 17:42:11 by calamber          #+#    #+#             */
/*   Updated: 2019/07/03 22:14:48 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

int		errorfunc(char *note, int v)
{
	ft_printf("%s", note);
	return (v);
}

int		init()
{
	if (!newwindow(&g_stuff.s) || 
			(g_stuff.image = new_image(g_stuff.s.mlx)) == NULL) 
	{	// Pass MLX into it's delete function which deletes everything created
		// in memory
        mlxdel();
		return (0);
    }
	return (1);
}

void	get_map(void)
{
	t_map *map;

	map = &g_stuff.map;
	int fourtytwo[19 * 11] = {	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
								0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
								0, 0, 10, 10, 0, 0, 10, 10, 0, 0, 0, 10, 10, 10, 10, 10, 0, 0, 0,
								0, 0, 10, 10, 0, 0, 10, 10, 0, 0, 0, 0, 0, 0, 0, 10, 10, 0, 0,
								0, 0, 10, 10, 0, 0, 10, 10, 0, 0, 0, 0, 0, 0, 0, 10, 10, 0, 0,
								0, 0, 10, 10, 10, 10, 10, 10, 0, 0, 0, 0, 10, 10, 10, 10, 0, 0, 0,
								0, 0, 0, 10, 10, 10, 10, 10, 0, 0, 0, 10, 10, 0, 0, 0, 0, 0, 0,
								0, 0, 0, 0, 0, 0, 10, 10, 0, 0, 0, 10, 10, 0, 0, 0, 0, 0, 0,
								0, 0, 0, 0, 0, 0, 10, 10, 0, 0, 0, 10, 10, 10, 10, 10, 10, 0, 0,
								0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
								0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	map->rows = 11;
	map->col = 19;
	map->v = (t_vox*)malloc(sizeof(t_vox) * map->rows * map->col);
	for (int y = 0; y < 11; y++)
	{
		for (int x = 0; x < 19; x++)
		{
			(*(map->v + (19 * y + x))).x = x;
			(*(map->v + (19 * y + x))).y = y;
			(*(map->v + (19 * y + x))).z = fourtytwo[19 * y + x];
		}
	}
}

int		main(int ac, char **av)
{
	g_rot_offset.x = 0.002;
	g_rot_offset.y = 0.002;
	if (ac == 2)
	{
		get_map();
		update_window();
	}
	else
		return (0);
	return (1);
}
