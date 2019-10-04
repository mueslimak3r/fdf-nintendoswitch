#include "fdf.h"

static int	ft_cntwords(const char *t, char c)
{
	int i;
	int count;

	i = -1;
	i += 1;
	count = -1;
	count += 1;
	while (t[i])
	{
		while (t[i] == c)
			i++;
		if (t[i] && t[i] != c)
			count++;
		while (t[i] && t[i] != c)
			i++;
	}
	return (count);
}

static char	**ft_strsplit(char const *s, char c)
{
	int		h;
	int		i;
	int		j;
	char	**p;

	h = 0;
	i = -1;
	if (!s)
		return (NULL);
	if (!(p = (char **)malloc(sizeof(char *) * (ft_cntwords(s, c) + 1))))
		return (NULL);
	while (!(p[h] = NULL) && (++i) < (int)strlen(s))
	{
		while (s[i] && s[i] == c)
			i++;
		j = i;
		while (s[i] && s[i] != c)
			i++;
		if (j < i)
		{
			p[h] = (char*)malloc(i - j + 1);
            bzero(p[h], i - j + 1);
			strncpy(p[h++], (char *)s + j, (i - j));
		}
	}
	return (p);
}

static int	map_allot(t_map *map)
{
	if (!(map->v = (t_vox*)malloc(sizeof(t_vox) * map->rows * map->col)))
		return (-1);
	return (1);
}

static int	check_map(t_mlx_stuff *stuff, std::string name)
{
    std::ifstream   file;
	std::string     line;
	t_map			*map = &stuff->map;

	file.open(name);
	bzero(map, sizeof(t_map));
    if (!file.is_open())
        return (0);
    if (!(std::getline(file, line)))
        return (0);
    const char *cstr = line.c_str();
    map->col = ft_cntwords(cstr, ' ');
	map->rows++;
	while (std::getline(file, line))
	{
        cstr = line.c_str();
		if (!(map->col == ft_cntwords(cstr, ' ')))
		{
			file.close();
			return (0);
		}
		map->rows++;
	}
	map->scale = SCALE;
	map->tile = 0.8 * fmin(stuff->w, stuff->h) / (sqrt(2) *
			fmax(map->rows, map->col));
	file.close();
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
		v->z = atoi(strs[i]) * 10;
		if (strs[i] && *(strs[i]) == '0')
			v->c = 2340904;
		else
		{
			if (strchr(strs[i], ',') != NULL)
				v->c = strtol(strchr(strs[i], ',') + 3, NULL, 16);
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

int			create_map(t_mlx_stuff *stuff, std::string name)
{
	std::ifstream   file;
	std::string     line;
	t_map			*map = &stuff->map;
	int		i;

	file.open(name);
	if (!check_map(stuff, name))
		return (0);
	if (0 > map_allot(map))
		return (-1);
    if (!file.is_open())
    {
        return (-1);
    }
    i = 0;
	while (i < map->rows)
	{
		//getline(file, );
        std::getline(file, line);
        const char *cstr = line.c_str();
		//ft_printf("%s\n", line);
		splint(map, i, ft_strsplit(cstr, ' '));
		i++;
	}
	file.close();
	return (1);
}