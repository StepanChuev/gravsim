#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "vp.h"
#include "body.h"
#include "bodyren.h"
#define FPS 60

int main(int argc, char const *argv[]){
	char ispause = 0;
	float scale = 1;
	float scale_step = 2;
	float min_radius = 2;
	size_t amount = 3;
	size_t fix_i = amount; // amount - no fixation
	size_t new_fix_i = fix_i;
	float movement_step = 10;
	Vec2 click;
	Vec2 shift;
	Vec2 movement;
	View_Port vp;
	Body *bodies = (Body *)malloc(amount * sizeof(Body));
	Body_Ren *bodies_ren = (Body_Ren *)malloc(amount * sizeof(Body_Ren));

	if (!bodies || !bodies_ren || !vp_init(&vp, -1, -1, "gravsim")){
		exit(EXIT_FAILURE);
	}

	shift.x = (float)vp.width / 2.0;
	shift.y = (float)vp.height / 2.0;

	bodies[0].x  = 0.0;
	bodies[0].y  = 0.0;
	bodies[0].vx = 0.0;
	bodies[0].vy = 0.0;
	bodies[0].m  = 20.0;

	bodies[1].x  = 0.0;
	bodies[1].y  = -100.0;
	bodies[1].vx = -0.2;
	bodies[1].vy = 0.0;
	bodies[1].m  = 1.0;

	bodies[2].x  = 0.0;
	bodies[2].y  = 100.0;
	bodies[2].vx = 0.3;
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
			switch (event.type){
			case SDL_QUIT:
				goto cleanup;

			case SDL_WINDOWEVENT:
				SDL_GetWindowSize(vp.screen, &vp.width, &vp.height);
				shift.x = vp.width / 2.0 + movement.x;
				shift.y = vp.height / 2.0 + movement.y;
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym){
				case 32:
					ispause ^= 1;
					break;

				case 'w':
					shift.y += movement_step;
					movement.y += movement_step;
					break;

				case 'a':
					shift.x += movement_step;
					movement.x += movement_step;
					break;

				case 's':
					shift.y -= movement_step;
					movement.y -= movement_step;
					break;

				case 'd':
					shift.x -= movement_step;
					movement.x -= movement_step;
					break;

				case SDLK_UP:
					scale *= scale_step;
					break;

				case SDLK_DOWN:
					scale /= scale_step;
					break;
				}

				break;
			
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == 1){
					click.x = (float)event.button.x - shift.x;
					click.y = (float)event.button.y - shift.y;

					new_fix_i = get_index_chosen_body(bodies, bodies_ren, 
						click, scale, min_radius, amount
					);

					if (new_fix_i != amount){
						fix_i = new_fix_i;
						movement.x = 0;
						movement.y = 0;
					}
				}

				else if (event.button.button == 3){
					fix_i = amount;
					movement.x = 0;
					movement.y = 0;
					shift.x = (float)vp.width / 2.0;
					shift.y = (float)vp.height / 2.0;
				}
			}
		}

		if (fix_i != amount){
			shift.x = (float)vp.width / 2.0 - scale * bodies[fix_i].x + movement.x;
			shift.y = (float)vp.height / 2.0 - scale * bodies[fix_i].y + movement.y;
		}

		SDL_SetRenderDrawColor(vp.ren, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(vp.ren);
		SDL_GetRendererOutputSize(vp.ren, &vp.width, &vp.height);
		render_bodies(&vp, bodies, bodies_ren, shift, scale, min_radius, amount);
		SDL_RenderPresent(vp.ren);

		if (ispause){
			continue;
		}

		clrscr();
		log_bodies(bodies, amount);

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