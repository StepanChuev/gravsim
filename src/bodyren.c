#include <SDL2/SDL.h>
#include <math.h>
#include "bodyren.h"

void render_body(View_Port *vp, Body *body, Body_Ren *body_ren, Vec2 shift, float scale, float min_r){
	SDL_SetRenderDrawColor(vp->ren, body_ren->r, 
		body_ren->g, body_ren->b, 0xFF
	);
	render_filled_circle(vp, body->x * scale + shift.x, body->y * scale + shift.y, fmaxf(body_ren->radius * scale, min_r));
}

void render_bodies(View_Port *vp, Body *bodies, Body_Ren *bodies_ren, Vec2 shift, float scale, float min_r, size_t len){
	for (size_t i = 0; i < len; i++){
		render_body(vp, bodies + i, bodies_ren + i, shift, scale, min_r);
	}
}

size_t get_index_chosen_body(double x, double y, Body *bodies, Body_Ren *bodies_ren, float scale, float min_r, size_t len){
	size_t i = 0;

	while (i < len){
		if (pow(bodies[i].x * scale - x, 2) + pow(bodies[i].y * scale - y, 2) <= 
			pow(fmaxf(bodies_ren[i].radius * scale, min_r), 2)){
			return i;
		}

		i++;
	}

	return len;
}