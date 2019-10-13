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
# include <string.h>
# include <stdio.h>
#include <fstream>
#include <iostream>
//#include <iomanip>

# define WHITE ((int)16777215)
# define SCALE ((double)-0.12)

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    # define RMASK ((uint32_t)0xff000000)
    # define GMASK ((uint32_t)0x00ff0000)
    # define BMASK ((uint32_t)0x0000ff00)
    # define AMASK ((uint32_t)0x000000ff)
#else
     # define RMASK ((uint32_t)0x000000ff)
     # define GMASK ((uint32_t)0x0000ff00)
     # define BMASK ((uint32_t)0x00ff0000)
     # define AMASK ((uint32_t)0xff000000)
#endif

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
	t_vox		rot_offset;
	double		scale;
	int			w;
	int			h;
	bool		v_flip;
	bool		h_flip;
}				t_mlx_stuff;

typedef struct s_RGB
{
        int		r;
        int		g;
        int		b;
} t_RGB;

int			create_map(t_mlx_stuff *stuff, std::string name);
void			map_destroy(t_map *map);

#endif
