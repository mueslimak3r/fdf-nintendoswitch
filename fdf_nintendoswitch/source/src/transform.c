/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 17:59:38 by calamber          #+#    #+#             */
/*   Updated: 2019/07/01 18:00:15 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static t_vox	mlx_rotate(t_vox *a)
{
	t_vox t_a;
	t_vox *r;

	r = &g_rot_offset;
	t_a.x = (cos(r->y) * a->x + sin(r->y) * a->z);
	t_a.z = (-sin(r->y) * a->x + cos(r->y) * a->z);
	t_a.y = (cos(r->x) * a->y - sin(r->x) * t_a.z);
	t_a.z = (sin(r->x) * a->y + cos(r->x) * t_a.z);
	t_a.c = a->c;
	return (t_a);
}

t_vox			mlx_project(t_vox p)
{
	t_vox t_a;

	p.x -= (g_stuff.map.col / 2);
	p.y -= (g_stuff.map.rows / 2);
	p.z *= SCALE;
	t_a = mlx_rotate(&p);
	t_a.x = (double)((W_XSIZE / 2)) + (t_a.x * g_stuff.map.tile);
	t_a.y = (double)((W_YSIZE / 2)) + (t_a.y * g_stuff.map.tile);
	return (t_a);
}
