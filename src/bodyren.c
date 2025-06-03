#include <SDL2/SDL.h>
#include "bodyren.h"

void render_body(View_Port *vp, Body *body, Body_Ren *body_ren, Vec2 shift){
	SDL_SetRenderDrawColor(vp->ren, body_ren->r, 
		body_ren->g, body_ren->b, 0xFF
	);
	render_filled_circle(vp, body->x + shift.x, body->y + shift.y, body_ren->radius);
}

void render_bodies(View_Port *vp, Body *bodies, Body_Ren *bodies_ren, Vec2 shift, size_t len){
	for (size_t i = 0; i < len; i++){
		render_body(vp, bodies + i, bodies_ren + i, shift);
	}
}

size_t get_index_chosen_body(double x, double y, Body *bodies, Body_Ren *bodies_ren, size_t len){
	size_t i = 0;

	while (i < len){
		if (pow(bodies[i].x - x, 2) + pow(bodies[i].y - y, 2) <= 
			pow(bodies_ren[i].radius, 2)){
			return i;
		}

		i++;
	}

	return len;
}