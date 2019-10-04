/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 17:28:33 by calamber          #+#    #+#             */
/*   Updated: 2019/07/03 22:08:50 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include <unistd.h>
# include <math.h>
# include <fcntl.h>
# include <limits.h>

# define WHITE ((int)16777215)
# define SCALE ((double)-0.12)

typedef struct	s_vox
{
	double		x;
	double		y;
	double		z;
	unsigned long	c;
}				t_vox;

typedef struct	s_map
{
	t_vox		*v;
	int			rows;
	int			col;
	double		scale;
	double		tile;
}				t_map;

typedef struct	s_mlx_stuff
{
	t_map		map;
	double		scale;
	int			w;
	int			h;
	bool		v_flip;
	bool		h_flip;
}				t_mlx_stuff;

typedef struct s_RGB
{
        double r;
        double g;
        double b;
} t_RGB;

t_vox			g_rot_offset;
t_mlx_stuff		g_stuff;

int				create_map(t_map *map, char *mname);
void			update_window(void);
int				errorfunc(char *note, int v);
void			map_destroy(t_map *map);
void			draw_line(t_vox a, t_vox b);
void			update_window(void);
t_vox			mlx_project(t_vox p);

#endif
