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

void update_vel(Body *bodies, size_t len, float G);
void update_coords(Body *bodies, size_t len, float G);

#endif