#ifndef VP_H
#define VP_H
#include <SDL2/SDL.h>

typedef struct View_Port {
	int width, height;
	SDL_Window *screen;
	SDL_Renderer *ren;
} View_Port;

int vp_init(View_Port *vp, int width, int height, const char *title);
void vp_cleanup(View_Port *vp);
void render_filled_circle(View_Port *vp, float x0, float y0, float r);

#endif