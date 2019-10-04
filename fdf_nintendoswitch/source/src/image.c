#include "../includes/fdf.h"

void    mlxdel(void)
{
	// If the mlx window is not null, then destroy it with the given function
	if (g_stuff.s.win != NULL)
		mlx_destroy_window(g_stuff.s.mlx, g_stuff.s.win);
	// If the cam object is not empty, it deletes it's address and the contents
	// If the image object is not empty, it deletes it's address and the contents
	if (g_stuff.image != NULL)
		del_image(g_stuff.image);
    if (g_stuff.s.map)
		map_destroy(g_stuff.s.map);
	// Deletes the contents and frees the address of mlx
}

void	image_set_pixel(t_image *image, int x, int y, int color)
{
	// Error checking
	if (x < 0 || x >= W_XSIZE || y < 0 || y >= W_YSIZE)
		return ;
	*(int *)(image->ptr + ((x + y * W_XSIZE) * image->bpp)) = color;
}

/*
 * Clears content of the memory address without freeing the pointer
 */
void	clear_image(t_image *image)
{
	// Sets the memory area of size height * width pointed to by the pointer to zero.
	ft_bzero(image->ptr, W_XSIZE * W_YSIZE * image->bpp);
}

/*
 * Deletes the image object associated with the mlx object and image.
 */
t_image		*del_image(t_image *img)
{
	// If image is not null
	if (img != NULL)
	{
		// if image pointed from image is not null
		if (img->image != NULL)
			mlx_destroy_image(g_stuff.s.mlx, img->image);
		// Delete the address of image
		ft_memdel((void **)&img);
	}
	return (NULL);
}

/*
 * Creates a new image in memory. Can be manipulated later
 */
t_image		*new_image(void)
{
	t_image		*img;

	// Allocates new area in memory for image
	if ((img = ft_memalloc(sizeof(t_image))) == NULL)
		return (NULL);
	// Creates a new image in memory. Can be manipulated later
	if ((img->image = mlx_new_image(g_stuff.s.mlx, W_XSIZE, W_YSIZE)) == NULL)
		return (del_image(img));
	// Function grabs the pointer address of image so that you can manipulate 
	// it later
	img->ptr = mlx_get_data_addr(img->image, &img->bpp, &img->stride, 
			&img->endian);
	// Bits per pixel: img->bpp = img->bpp / 8;
	img->bpp /= 8;
	return (img);
}