#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>

#define MAP_NB 15

#include "fdf.h"

int         get_map(t_mlx_stuff *stuff, int nb)
{
    t_map   *map;
    int     ret = 0;
    map = &stuff->map;
    if (stuff->map.v)
    {
        free(stuff->map.v);
        stuff->map.v = NULL;
    }
    std::string files[15] = { "fdf_maps/10-2.fdf", "fdf_maps/10-70.fdf", "fdf_maps/20-60.fdf", "fdf_maps/42.fdf", "fdf_maps/50-4.fdf",
                    "fdf_maps/100-6.fdf", "fdf_maps/basictest.fdf", "fdf_maps/elem.fdf", "fdf_maps/mars.fdf", "fdf_maps/pentenegpos.fdf",
                    "fdf_maps/plat.fdf", "fdf_maps/pnp_flat.fdf", "fdf_maps/pylone.fdf", "fdf_maps/pyra.fdf", "fdf_maps/pyramide.fdf" };
    if (nb > -1 && nb < 15)
        ret = create_map(stuff, files[nb]);
    if (ret != 1)
        return (0);
    stuff->rot_offset.x = 0.002;
	stuff->rot_offset.y = 0.002;
    map->tile = 0.8 * fmin(stuff->w, stuff->h) / (sqrt(2) * fmax(map->rows, map->col));
    return (1);
}

static t_vox	mlx_rotate(t_mlx_stuff *stuff, t_vox *a)
{
	t_vox t_a;
	t_vox *r;

	r = &stuff->rot_offset;
	t_a.x = (cos(r->y) * a->x + sin(r->y) * a->z);
	t_a.z = (-sin(r->y) * a->x + cos(r->y) * a->z);
	t_a.y = (cos(r->x) * a->y - sin(r->x) * t_a.z);
	t_a.z = (sin(r->x) * a->y + cos(r->x) * t_a.z);
	t_a.c = a->c;
	return (t_a);
}

t_vox			mlx_project(t_mlx_stuff *stuff, t_vox p)
{
	t_vox t_a;

	p.x -= (stuff->map.col / 2);
	p.y -= (stuff->map.rows / 2);
	p.z *= stuff->scale;
	t_a = mlx_rotate(stuff, &p);
	t_a.x = (double)((stuff->w / 2)) + (t_a.x * stuff->map.tile);
	t_a.y = (double)((stuff->h / 2)) + (t_a.y * stuff->map.tile);
	return (t_a);
}

/*
t_RGB color_converter (int hexValue)
{
    t_RGB rgbColor;
    rgbColor.r = hexValue >> 16;//((hexValue >> 16) & 0xFF) / 255.0; // Extract the RR byte
    rgbColor.g = (hexValue & 0x00ff00) >> 8;//((hexValue >> 8) & 0xFF) / 255.0; // Extract the GG byte
    rgbColor.b = (hexValue & 0x0000ff);//((hexValue) & 0xFF) / 255.0; // Extract the BB byte
    return (rgbColor); 
}
*/

static int	ft_abs(int n)
{
	return ((n > 0) ? n : (n * -1));
}

uint32_t    make_color(int hexValue)
{
    uint32_t ret;
    ret = hexValue >> 16; // r
    ret <<= 16;
    ret |= (hexValue & 0x00ff00) >> 8; // g
    ret <<= 16;
    ret |= (hexValue & 0x0000ff); //b
    ret |= 255; // a
    /*
    rgbColor.r = hexValue >> 16;//((hexValue >> 16) & 0xFF) / 255.0; // Extract the RR byte
    rgbColor.g = (hexValue & 0x00ff00) >> 8;//((hexValue >> 8) & 0xFF) / 255.0; // Extract the GG byte
    rgbColor.b = (hexValue & 0x0000ff);//((hexValue) & 0xFF) / 255.0; // Extract the BB byte
    */
    return (ret);
}

void		dda(t_mlx_stuff *mlx, uint32_t *pixels, t_vox *start, t_vox *end, int color)
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
        {
            if (start->x >= 0 && start->x < mlx->w && start->y >= 0 && start->y < mlx->h)
			    pixels[mlx->w * (int)start->y + (int)start->x] = make_color(color);
        }
        start->x += dx / (float)steps;
		start->y += dy / (float)steps;
		i++;
	}
}

void draw_line(t_mlx_stuff *stuff, uint32_t *pixels, int x1, int y1, int x2, int y2)
{
    t_map *map = &stuff->map;
    t_vox *a;
    t_vox *b;
    t_vox p1;
    t_vox p2;

    a = &((map->v)[map->col * y1 + x1]);
    b = &((map->v)[map->col * y2 + x2]);
    p1 = mlx_project(stuff, (map->v)[map->col * y1 + x1]);
    p2 = mlx_project(stuff, (map->v)[map->col * y2 + x2]);
    p1 = p1;
    p2 = p2;
    int color = (a->z != b->z) ? 14358738 - ((fmax(a->z, b->z) - fmin(a->z, b->z)) * 150) : a->c;
    dda(stuff, pixels, &p1, &p2, color);
    //SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
    //SDL_RenderDrawLine(renderer, (int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y);
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void    init_mlx_stuff(t_mlx_stuff *stuff, int h, int w)
{
    stuff->rot_offset.x = 0.002;
	stuff->rot_offset.y = 0.002;
    stuff->scale = SCALE;
    stuff->map.v = NULL;
    stuff->h = h;
    stuff->w = w;
   
}

void fdf_init(t_mlx_stuff *stuff, SDL_Surface **surface, SDL_Texture **texture, SDL_Window **window, SDL_Renderer **renderer)
{

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        SDL_Log("SDL_Init: %s\n", SDL_GetError());
        exit (-1);
    }

    // create an SDL window (OpenGL ES2 always enabled)
    // when SDL_FULLSCREEN flag is not set, viewport is automatically handled by SDL (use SDL_SetWindowSize to "change resolution")
    // available switch SDL2 video modes :
    // 1920 x 1080 @ 32 bpp (SDL_PIXELFORMAT_RGBA8888)
    // 1280 x 720 @ 32 bpp (SDL_PIXELFORMAT_RGBA8888)
    *window = SDL_CreateWindow("sdl2_gles2", 0, 0, 1280, 720, 0);
    if (!*window) {
        SDL_Log("SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        exit (-1);
    }

    // create a renderer (OpenGL ES2)
    *renderer = SDL_CreateRenderer(*window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!*renderer) {
        SDL_Log("SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        exit (-1);
    }

    *surface = SDL_CreateRGBSurface(0, 1280, 720, 32, 0, 0, 0, 0);
    if (!*surface)
    {
        SDL_Log("SDL_CreateRGBSurface: %s\n", SDL_GetError());
        SDL_DestroyRenderer(*renderer);
        SDL_DestroyWindow(*window);
        SDL_Quit();
        exit (-1);
    }

    *texture = SDL_CreateTextureFromSurface(*renderer, *surface);
    if (!*texture)
    {
        SDL_Log("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
        SDL_FreeSurface(*surface);
        SDL_DestroyRenderer(*renderer);
        SDL_DestroyWindow(*window);
        SDL_Quit();
        exit (-1);
    }
    // open CONTROLLER_PLAYER_1 and CONTROLLER_PLAYER_2
    // when railed, both joycons are mapped to joystick #0,
    // else joycons are individually mapped to joystick #0, joystick #1, ...
    // https://github.com/devkitPro/SDL/blob/switch-sdl2/src/joystick/switch/SDL_sysjoystick.c#L45
    for (int i = 0; i < 2; i++) {
        if (SDL_JoystickOpen(i) == NULL) {
            SDL_Log("SDL_JoystickOpen: %s\n", SDL_GetError());
            SDL_DestroyTexture(*texture);
            SDL_FreeSurface(*surface);
            SDL_DestroyRenderer(*renderer);
            SDL_DestroyWindow(*window);
            SDL_Quit();
            exit (-1);
        }
    }
    init_mlx_stuff(stuff, 720, 1280);
}

int main(int argc, char *argv[])
{
    SDL_Event       event;
    SDL_Surface     *surface = nullptr;
    SDL_Texture     *texture = nullptr;
    SDL_Window      *window = nullptr;
    SDL_Renderer    *renderer = nullptr;
    t_mlx_stuff     stuff;

    int done = 0, w = 1280, h = 720;
    // mandatory at least on switch, else gfx is not properly closed
    fdf_init(&stuff, &surface, &texture, &window, &renderer);
    int chosen_map = 0;
    if (!(get_map(&stuff, chosen_map)))
        done = 1;
    t_map *map = &stuff.map;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    while (!done) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_JOYAXISMOTION:
                    SDL_Log("Joystick %d axis %d value: %d\n",
                            event.jaxis.which,
                            event.jaxis.axis, event.jaxis.value);
                    if( event.jaxis.which == 0 )
                    {
                        const int JOYSTICK_DEAD_ZONE = 8000;
                        //X axis motion
                        if( event.jaxis.axis == 0 )
                        {
                            //Left of dead zone
                            if( event.jaxis.value < -JOYSTICK_DEAD_ZONE )
                            {
                                stuff.rot_offset.y += 0.06;
                            }
                            //Right of dead zone
                            else if( event.jaxis.value > JOYSTICK_DEAD_ZONE )
                            {
                                stuff.rot_offset.y -= 0.06;
                            }
                        }
                        else if( event.jaxis.axis == 1 )
                        {
                                //Below of dead zone
                                if( event.jaxis.value < -JOYSTICK_DEAD_ZONE )
                                {
                                    stuff.rot_offset.x -= 0.06;
                                }
                                //Above of dead zone
                                else if( event.jaxis.value > JOYSTICK_DEAD_ZONE )
                                {
                                    stuff.rot_offset.x += 0.06;
                                }
                        }
                    }

                    break;

                case SDL_JOYBUTTONDOWN:
                    SDL_Log("Joystick %d button %d down\n",
                            event.jbutton.which, event.jbutton.button);
                    // https://github.com/devkitPro/SDL/blob/switch-sdl2/src/joystick/switch/SDL_sysjoystick.c#L52
                    // seek for joystick #0
                    if (event.jbutton.which == 0) {
                        /*
                        if (event.jbutton.button == 0) {
                            // (A) button down, switch resolution
                            if(w == 1920) {
                                SDL_SetWindowSize(window, 1280, 720);
                            }
                            else {
                                SDL_SetWindowSize(window, 1920, 1080);
                            }
                            stuff.h = h;
                            stuff.w = w;
                        }
                        */
                        if (event.jbutton.button == 10) {
                            // (+) button down
                            done = 1;
                        }
                        else if (event.jbutton.button == 11) {
                            // (-) button down
                            chosen_map = (chosen_map + 1 >= MAP_NB) ? 0 : chosen_map + 1;
                            if (!(get_map(&stuff, chosen_map)))
                            {
                                if (stuff.map.v)
                                    free(stuff.map.v);
                                SDL_DestroyTexture(texture);
                                SDL_FreeSurface(surface);
                                SDL_DestroyRenderer(renderer);
                                SDL_DestroyWindow(window);
                                SDL_Quit();
                                exit(0);
                            }
                        }
                        else if (event.jbutton.button == 9) {
                            // zoom in
                            stuff.map.tile *= 1.2;
                        }
                        else if (event.jbutton.button == 8) {
                            // zoom out
                            stuff.map.tile *= 0.8;
                        }
                        else if (event.jbutton.button == 13) {
                            // zoom in
                            stuff.scale *= 1.2;
                        }
                        else if (event.jbutton.button == 15) {
                            // zoom out
                            stuff.scale *= 0.8;
                        }
                        /*
                        else if (event.jbutton.button == 12) {
                            // rotate left
                            stuff.rot_offset.y += 0.06;
                        }
                        else if (event.jbutton.button == 14) {
                            // rotate right
                            stuff.rot_offset.y -= 0.06;
                        }
                        else if (event.jbutton.button == 13) {
                            // rotate up
                            stuff.rot_offset.x -= 0.06;
                        }
                        else if (event.jbutton.button == 15) {
                            // rotate down
                            stuff.rot_offset.x += 0.06;
                        }
                        */
                    }
                    break;

                default:
                    break;
            }
        }
        SDL_GetWindowSize(window, &w, &h);
        uint32_t *pixels = (uint32_t *)surface->pixels;
        memset(pixels, 0, w * h * sizeof(uint32_t));
        
        for (int my = 0; my < map->rows; my++)
        {
            for (int mx = 0; mx < map->col; mx++)
            {
                if (mx - 1 >= 0)
                    draw_line(&stuff, pixels, mx, my, mx - 1, my);
                if (my - 1 >= 0)
                    draw_line(&stuff, pixels, mx, my, mx, my - 1);
                
            }
        }
        SDL_UpdateTexture(texture, nullptr, surface->pixels, surface->pitch);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }
    if (stuff.map.v)
        free(stuff.map.v);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
