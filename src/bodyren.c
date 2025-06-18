#include <SDL2/SDL.h>
#include <math.h>
#include "bodyren.h"

void render_body(View_Port *vp, Body *body, Body_Ren *body_ren, 
	Vec2 shift, float scale)
{
	float x = body->x * scale + shift.x;
	float y = body->y * scale + shift.y;
	float r = body_ren->radius * scale;

	if (
		x + r < 0.0 || y + r < 0.0 || 
		x - r > (float)vp->width || y - r > (float)vp->height
	){
		return;
	}

	SDL_SetRenderDrawColor(vp->ren, body_ren->r, 
		body_ren->g, body_ren->b, 0xFF
	);
	render_filled_circle(vp, x, y, r);
}

void render_bodies(View_Port *vp, Body *bodies, Body_Ren *bodies_ren, 
	Vec2 shift, float scale, size_t len)
{
	for (size_t i = 0; i < len; i++){
		render_body(vp, bodies + i, bodies_ren + i, shift, scale);
	}
}

void log_bodies(Body *bodies, size_t len){
	for (size_t i = 0; i < len; i++){
		printf("BODY %lu\n", i);
		printf("\tx: %f\ty: %f\n\tvx: %f\tvy: %f\n\tm: %f\n", 
			bodies[i].x, bodies[i].y, bodies[i].vx, bodies[i].vy, bodies[i].m
		);
	}
}

size_t get_index_chosen_body(Body *bodies, Body_Ren *bodies_ren, 
	Vec2 click, float scale, size_t len)
{
	size_t i = 0;

	while (i < len){
		if (powf(bodies[i].x * scale - click.x, 2) + powf(bodies[i].y * scale - click.y, 2) <= 
			powf(bodies_ren[i].radius * scale, 2)){
			return i;
		}

		i++;
	}

	return len;
}