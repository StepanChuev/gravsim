#include <math.h>
#include <stdlib.h>
#include "body.h"

Vec2 *calc_force(Body *body1, Body *body2, float G){
	float diffx_sqr = powf(body2->x - body1->x, 2.0);
	float diffy_sqr = powf(body2->y - body1->y, 2.0);
	float r_sqr = diffx_sqr + diffy_sqr;
	float F = G * body1->m * body2->m / r_sqr;

	Vec2 *F_vec = (Vec2 *)malloc(sizeof(Vec2));

	if (F_vec == NULL){
		return NULL;
	}

	F_vec->x = F * sqrtf(diffx_sqr / r_sqr);
	F_vec->y = F * sqrtf(diffy_sqr / r_sqr);

	return F_vec;
}

// calc accel for body1
Vec2 *calc_accel(Body *body1, Body *body2, float G){
	Vec2 *res = calc_force(body1, body2, G);

	if (res == NULL){
		return NULL;
	}

	if (body1->x > body2->x){
		res->x *= -1.0;
	}

	if (body1->y > body2->y){
		res->y *= -1.0;
	}

	res->x /= body1->m;
	res->y /= body1->m;

	return res;
}

void update_vel(Body *bodies, size_t len, float G){
	Vec2 *a = NULL;

	for (size_t i = 0; i < len; i++){
		for (size_t j = 0; j < len; j++){
			if (i == j){
				continue;
			}

			a = calc_accel(bodies + i, bodies + j, G);

			if (a == NULL){
				continue;
			}

			bodies[i].vx += a->x;
			bodies[i].vy += a->y;

			free(a);
		}
	}
}

void update_coords(Body *bodies, size_t len, float G){
	update_vel(bodies, len, G);

	for (size_t i = 0; i < len; i++){
		bodies[i].x += bodies[i].vx;
		bodies[i].y += bodies[i].vy;
	}
}