#ifndef BODYREN_H
#define BODYREN_H
#include "vp.h"
#include "body.h"

typedef struct Body_Ren {
	unsigned char r, g, b;
	float radius;
} Body_Ren;

void render_body(View_Port *vp, Body *body, Body_Ren *body_ren, Vec2 shift, float scale, float min_r);
void render_bodies(View_Port *vp, Body *bodies, Body_Ren *bodies_ren, Vec2 shift, float scale, float min_r, size_t len);
size_t get_index_chosen_body(double x, double y, Body *bodies, Body_Ren *bodies_ren, float scale, float min_r, size_t len);

#endif