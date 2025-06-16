#ifndef BODY_H
#define BODY_H

typedef struct Vec2 {
	float x, y;
} Vec2;

typedef struct Body {
	float x, y;
	float vx, vy;
	float m;
} Body;

Vec2 *calc_force(Body *body1, Body *body2, float G);
Vec2 *calc_accel(Body *body1, Body *body2, float G); // calc accel for body1
void update_vel(Body *bodies, size_t len, float G);
void update_coords(Body *bodies, size_t len, float G);

#endif