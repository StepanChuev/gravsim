#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <SDL2/SDL.h>
#include "vp.h"
#include "io.h"
#include "body.h"
#include "bodyren.h"
#define FPS 60

int main(int argc, char *argv[]){
	Flags default_flags = {0, 1, -1, -1, 2.0, 10.0, SIZE_MAX};
	Flags *flags = get_flags_from_args(argc, argv, &default_flags);
	float scale = 1.0;
	float max_scale = 1024.0;
	size_t amount = 3;
	size_t new_fix_i = flags->fix_i;
	Vec2 click;
	Vec2 shift;
	Vec2 movement;
	View_Port vp;
	Body *bodies = (Body *)malloc(amount * sizeof(Body));
	Body_Ren *bodies_ren = (Body_Ren *)malloc(amount * sizeof(Body_Ren));

	if (!bodies || !bodies_ren || !vp_init(&vp, flags->width, flags->height, "gravsim")){
		exit(EXIT_FAILURE);
	}

	flags->width = vp.width;
	flags->height = vp.height;
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
					flags->ispause ^= 1;
					break;

				case 'w':
					shift.y += flags->move_step;
					movement.y += flags->move_step;
					break;

				case 'a':
					shift.x += flags->move_step;
					movement.x += flags->move_step;
					break;

				case 's':
					shift.y -= flags->move_step;
					movement.y -= flags->move_step;
					break;

				case 'd':
					shift.x -= flags->move_step;
					movement.x -= flags->move_step;
					break;

				case SDLK_UP:
					scale *= flags->scale_step;
					scale = (scale < max_scale) ? scale : max_scale;
					break;

				case SDLK_DOWN:
					scale /= flags->scale_step;
					break;
				}

				break;
			
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == 1){
					click.x = (float)event.button.x - shift.x;
					click.y = (float)event.button.y - shift.y;

					new_fix_i = get_index_chosen_body(bodies, bodies_ren, 
						click, scale, amount
					);

					if (new_fix_i != amount){
						flags->fix_i = new_fix_i;
						movement.x = 0;
						movement.y = 0;
					}
				}

				else if (event.button.button == 3){
					flags->fix_i = amount;
					movement.x = 0;
					movement.y = 0;
					shift.x = (float)vp.width / 2.0;
					shift.y = (float)vp.height / 2.0;
				}
			}
		}

		if (flags->fix_i < amount){
			shift.x = (float)vp.width / 2.0 - scale * bodies[flags->fix_i].x + movement.x;
			shift.y = (float)vp.height / 2.0 - scale * bodies[flags->fix_i].y + movement.y;
		}

		SDL_SetRenderDrawColor(vp.ren, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(vp.ren);
		SDL_GetRendererOutputSize(vp.ren, &vp.width, &vp.height);
		render_bodies(&vp, bodies, bodies_ren, shift, scale, amount);
		SDL_RenderPresent(vp.ren);

		if (flags->ispause){
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
	free(flags);

	return 0;
}