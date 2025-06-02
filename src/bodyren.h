#ifndef BODYREN_H
#define BODYREN_H
#include "vp.h"
#include "body.h"

typedef struct Body_Ren {
	unsigned char r, g, b;
	int radius;
} Body_Ren;

void render_body(View_Port *vp, Body *body, Body_Ren *body_ren, Vec2 shift);
void render_bodies(View_Port *vp, Body *bodies, Body_Ren *bodies_ren, Vec2 shift, size_t len);

#endif