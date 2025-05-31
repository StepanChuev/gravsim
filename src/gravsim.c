#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "vp.h"
#include "body.h"
#include "bodyren.h"
#define FPS 60

int main(int argc, char const *argv[]){
	size_t amount = 3;
	View_Port vp;
	Body *bodies = (Body *)malloc(amount * sizeof(Body));
	Body_Ren *bodies_ren = (Body_Ren *)malloc(amount * sizeof(Body_Ren));

	if (!bodies || !bodies_ren || !vp_init(&vp, -1, -1, "gravsim")){
		exit(EXIT_FAILURE);
	}

	printf("%d %d\n", vp.width, vp.height);

	bodies[0].x  = (double)vp.width / 2.0;
	bodies[0].y  = (double)vp.height / 2.0;
	bodies[0].vx = 0.0;
	bodies[0].vy = 0.0;
	bodies[0].m  = 20.0;

	bodies[1].x  = (double)vp.width / 2.0;
	bodies[1].y  = (double)vp.height / 2.0 - 100.0;
	bodies[1].vx = -0.2;
	bodies[1].vy = 0.0;
	bodies[1].m  = 1.0;

	bodies[2].x  = (double)vp.width / 2.0;
	bodies[2].y  = (double)vp.height / 2.0 + 100.0;
	bodies[2].vx = 0.2;
	bodies[2].vy = 0.0;
	bodies[2].m  = 1.0;

	bodies_ren[0].r = 0xFF;
	bodies_ren[0].g = 0xFF;
	bodies_ren[0].b = 0x00;
	bodies_ren[0].radius = 7;

	bodies_ren[1].r = 0xFF;
	bodies_ren[1].g = 0x00;
	bodies_ren[1].b = 0x00;
	bodies_ren[1].radius = 5;

	bodies_ren[2].r = 0x00;
	bodies_ren[2].g = 0x00;
	bodies_ren[2].b = 0xFF;
	bodies_ren[2].radius = 5;

	Uint32 start, elapsed, estimated = 1000 / FPS;
	SDL_Event event;

	for (;;){
		start = SDL_GetTicks();

		while (SDL_PollEvent(&event)){
			if (event.type == SDL_QUIT){
				goto cleanup;
			}
		}

		SDL_SetRenderDrawColor(vp.ren, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(vp.ren);
		SDL_GetRendererOutputSize(vp.ren, &vp.width, &vp.height);
		render_bodies(&vp, bodies, bodies_ren, amount);
		SDL_RenderPresent(vp.ren);

		update_coords(bodies, amount, 1);

		elapsed = SDL_GetTicks() - start;

		if (elapsed < estimated){
			SDL_Delay(estimated - elapsed);
		}
	}

cleanup:
	free(bodies_ren);
	free(bodies);
	vp_cleanup(&vp);

	return 0;
}