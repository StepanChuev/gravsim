#ifndef BODY_H
#define BODY_H

typedef struct Vec2 {
	double x, y;
} Vec2;

typedef struct Body {
	double x, y;
	double vx, vy;
	double m;
} Body;

Vec2 *calc_force(Body *body1, Body *body2, double G);
Vec2 *calc_accel(Body *body1, Body *body2, double G); // calc accel for body1
void update_vel(Body *bodies, size_t len, double G);
void update_coords(Body *bodies, size_t len, double G);

#endif