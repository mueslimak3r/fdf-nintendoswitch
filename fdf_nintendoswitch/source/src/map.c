/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 17:45:44 by calamber          #+#    #+#             */
/*   Updated: 2019/07/01 18:49:33 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static int	map_allot(t_map *map)
{
	if (!(map->v = (t_vox*)ft_memalloc(sizeof(t_vox) * map->rows * map->col)))
		return (errorfunc("error: can't malloc vox", -1));
	return (1);
}

static int	check_map(t_map *map, char *mname)
{
	int		fd;
	char	*line;

	line = NULL;
	ft_bzero(map, sizeof(t_map));
	if (0 > (fd = open(mname, O_RDONLY)))
		return (errorfunc("error: cannot open map\n", 0));
	if (!(get_next_line(fd, &line)))
		return (errorfunc("error: read error\n", 0));
	map->col = ft_cntwords(line, ' ');
	map->rows++;
	free(line);
	while (get_next_line(fd, &line))
	{
		if (!(map->col == ft_cntwords(line, ' ')))
		{
			return (errorfunc("error: bad map formatting", 0));
		}
		map->rows++;
		free(line);
	}
	map->scale = SCALE;
	map->tile = 0.8 * fmin(W_XSIZE, W_YSIZE) / (sqrt(2) *
			fmax(map->rows, map->col));
	return (1);
}

static void	splint(t_map *map, int row, char **strs)
{
	int		i;
	t_vox	*v;

	i = -1;
	while (++i < map->col)
	{
		v = &map->v[map->col * row + i];
		v->x = i;
		v->y = row;
		v->z = ft_atoi_base(strs[i], 10) * 10;
		if (strs[i] && *(strs[i]) == '0')
			v->c = 2340904;
		else
		{
			if (ft_strchr(strs[i], ',') != NULL)
				v->c = ft_atoi_base(ft_strchr(strs[i], ',') + 3, 16);
			else
				v->c = 2248959 + ((int)v->z * 50);
		}
		free(strs[i]);
	}
	free(strs);
}

void		map_destroy(t_map *map)
{
	if (map && map->v)
	{
		free(map->v);
	}
}

int			create_map(t_map *map, char *mname)
{
	int		fd;
	char	*line;
	int		i;

	if (!check_map(map, mname))
		return (0);
	if (0 > map_allot(map))
		return (errorfunc("error: map alloc", -1));
	if (0 > (fd = open(mname, O_RDONLY)))
		return (errorfunc("error: can't open file", -1));
	i = 0;
	while (i < map->rows)
	{
		get_next_line(fd, &line);
		//ft_printf("%s\n", line);
		splint(map, i, ft_strsplit(line, ' '));
		free(line);
		i++;
	}
	return (1);
}
