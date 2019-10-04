#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>

#define MAP_NB 1

#include "fdf.h"

void	map_ft(void)
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
            t_vox *v = map->v + (19 * y + x);
			v->x = x;
			v->y = y;
			v->z = fourtytwo[19 * y + x];
            v->c = 2248959 + ((long)v->z * 50);
		}
	}
}

void	get_map(int nb)
{
    t_map *map;

    map = &g_stuff.map;
    if (g_stuff.map.v)
    {
        free(g_stuff.map.v);
        g_stuff.map.v = NULL;
    }
    switch (nb)
    {
        case 0:
            map_ft();
    }
    map->tile = 0.8 * fmin(g_stuff.w, g_stuff.h) / (sqrt(2) * fmax(map->rows, map->col));
    g_stuff.scale = SCALE;
}

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
	p.z *= g_stuff.scale;
	t_a = mlx_rotate(&p);
	t_a.x = (double)((g_stuff.w / 2)) + (t_a.x * g_stuff.map.tile);
	t_a.y = (double)((g_stuff.h / 2)) + (t_a.y * g_stuff.map.tile);
	return (t_a);
}

t_RGB color_converter (int hexValue)
{
    t_RGB rgbColor;
    rgbColor.r = hexValue >> 16;//((hexValue >> 16) & 0xFF) / 255.0; // Extract the RR byte
    rgbColor.g = (hexValue & 0x00ff00) >> 8;//((hexValue >> 8) & 0xFF) / 255.0; // Extract the GG byte
    rgbColor.b = (hexValue & 0x0000ff);//((hexValue) & 0xFF) / 255.0; // Extract the BB byte
    return (rgbColor); 
}

void draw_line(SDL_Renderer *renderer, int x1, int y1, int x2, int y2)
{
    t_map *map = &g_stuff.map;
    t_vox *a;
    t_vox *b;
    t_vox p1;
    t_vox p2;

    a = &((map->v)[map->col * y1 + x1]);
    b = &((map->v)[map->col * y2 + x2]);
    p1 = mlx_project((map->v)[map->col * y1 + x1]);
    p2 = mlx_project((map->v)[map->col * y2 + x2]);
    t_RGB c = color_converter((a->z != b->z) ? 14358738 - ((fmax(a->z, b->z) - fmin(a->z, b->z)) * 150) : a->c);
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
    SDL_RenderDrawLine(renderer, (int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y);
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

int main(int argc, char *argv[])
{
    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *renderer;
    int done = 0, w = 1280, h = 720;

    // mandatory at least on switch, else gfx is not properly closed
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        SDL_Log("SDL_Init: %s\n", SDL_GetError());
        return -1;
    }

    // create an SDL window (OpenGL ES2 always enabled)
    // when SDL_FULLSCREEN flag is not set, viewport is automatically handled by SDL (use SDL_SetWindowSize to "change resolution")
    // available switch SDL2 video modes :
    // 1920 x 1080 @ 32 bpp (SDL_PIXELFORMAT_RGBA8888)
    // 1280 x 720 @ 32 bpp (SDL_PIXELFORMAT_RGBA8888)
    window = SDL_CreateWindow("sdl2_gles2", 0, 0, 1280, 720, 0);
    if (!window) {
        SDL_Log("SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // create a renderer (OpenGL ES2)
    renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_Log("SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // open CONTROLLER_PLAYER_1 and CONTROLLER_PLAYER_2
    // when railed, both joycons are mapped to joystick #0,
    // else joycons are individually mapped to joystick #0, joystick #1, ...
    // https://github.com/devkitPro/SDL/blob/switch-sdl2/src/joystick/switch/SDL_sysjoystick.c#L45
    for (int i = 0; i < 2; i++) {
        if (SDL_JoystickOpen(i) == NULL) {
            SDL_Log("SDL_JoystickOpen: %s\n", SDL_GetError());
            SDL_Quit();
            return -1;
        }
    }
    int chosen_map = 0;
    g_rot_offset.x = 0.002;
	g_rot_offset.y = 0.002;
    g_stuff.scale = SCALE;
    g_stuff.map.v = NULL;
    g_stuff.h = h;
    g_stuff.w = w;
    get_map(chosen_map);
    t_map *map = &g_stuff.map;
    
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
                                g_rot_offset.y += 0.06;
                            }
                            //Right of dead zone
                            else if( event.jaxis.value > JOYSTICK_DEAD_ZONE )
                            {
                                g_rot_offset.y -= 0.06;
                            }
                        }
                        else if( event.jaxis.axis == 1 )
                        {
                                //Below of dead zone
                                if( event.jaxis.value < -JOYSTICK_DEAD_ZONE )
                                {
                                    g_rot_offset.x -= 0.06;
                                }
                                //Above of dead zone
                                else if( event.jaxis.value > JOYSTICK_DEAD_ZONE )
                                {
                                    g_rot_offset.x += 0.06;
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
                        if (event.jbutton.button == 0) {
                            // (A) button down, switch resolution
                            if(w == 1920) {
                                SDL_SetWindowSize(window, 1280, 720);
                            }
                            else {
                                SDL_SetWindowSize(window, 1920, 1080);
                            }
                            g_stuff.h = h;
                            g_stuff.w = w;
                        }
                        else if (event.jbutton.button == 10) {
                            // (+) button down
                            done = 1;
                        }
                        else if (event.jbutton.button == 11) {
                            // (-) button down
                            chosen_map = (chosen_map + 1 >= MAP_NB) ? 0 : chosen_map + 1;
                            get_map(chosen_map);
                        }
                        else if (event.jbutton.button == 9) {
                            // zoom in
                            g_stuff.map.tile *= 1.2;
                        }
                        else if (event.jbutton.button == 8) {
                            // zoom out
                            g_stuff.map.tile *= 0.8;
                        }
                        else if (event.jbutton.button == 13) {
                            // zoom in
                            g_stuff.scale *= 1.2;
                        }
                        else if (event.jbutton.button == 15) {
                            // zoom out
                            g_stuff.scale *= 0.8;
                        }
                        /*
                        else if (event.jbutton.button == 12) {
                            // rotate left
                            g_rot_offset.y += 0.06;
                        }
                        else if (event.jbutton.button == 14) {
                            // rotate right
                            g_rot_offset.y -= 0.06;
                        }
                        else if (event.jbutton.button == 13) {
                            // rotate up
                            g_rot_offset.x -= 0.06;
                        }
                        else if (event.jbutton.button == 15) {
                            // rotate down
                            g_rot_offset.x += 0.06;
                        }
                        */
                    }
                    break;

                default:
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // fill window bounds
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_GetWindowSize(window, &w, &h);
        SDL_Rect f = {0, 0, w, h};
        SDL_RenderFillRect(renderer, &f);
        for (int my = 0; my < map->rows; my++)
        {
            for (int mx = 0; mx < map->col; mx++)
            {
                if (mx - 1 >= 0)
                    draw_line(renderer, mx, my, mx - 1, my);
                if (my - 1 >= 0)
                    draw_line(renderer, mx, my, mx, my - 1);
                
            }
        }
        SDL_RenderPresent(renderer);
    }
    if (g_stuff.map.v)
        free(g_stuff.map.v);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
