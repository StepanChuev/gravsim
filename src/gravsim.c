#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <limits.h>
#include <SDL2/SDL.h>
#include "vp.h"
#include "io.h"
#include "body.h"
#include "bodyren.h"
#include "system.h"
#define SAVE_FILEPATH_SIZE 128
#define FPS 60

int main(int argc, char *argv[]){
	Flags default_flags = {0, 1, -1, -1, 2.0, 10.0, SIZE_MAX, NULL, ".", "sys"};
	Flags *flags = get_flags_from_args(argc, argv, &default_flags);
	float scale = 1.0;
	float max_scale = 1024.0;
	size_t new_fix_i = flags->fix_i;
	Vec2 click;
	Vec2 shift = {0, 0};
	Vec2 movement = {0, 0};
	View_Port vp;
	char save_filepath[SAVE_FILEPATH_SIZE];
	System *sys = NULL;

	if (!flags)
		goto cleanup;

	if (!flags->sys_filepath || !vp_init(&vp, flags->width, flags->height, "gravsim"))
		goto cleanup;
	

	flags->width = vp.width;
	flags->height = vp.height;
	shift.x = (float)vp.width / 2.0;
	shift.y = (float)vp.height / 2.0;

	sys = read_system_from_file(flags->sys_filepath);

	if (!sys)
		goto cleanup;

	if (!sys->bodies || !sys->bodies_ren)
		goto cleanup;

	Uint32 start, elapsed, estimated = 1000 / FPS;
	SDL_Event event;

	for (;; sys->iter++){
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

				case 'l':
					snprintf(
						save_filepath, SAVE_FILEPATH_SIZE, "%s/%s_%u.conf",
						flags->save_dir, flags->save_pref, sys->iter
					);
					write_system_to_file(sys, save_filepath);
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

					new_fix_i = get_index_chosen_body(sys->bodies, sys->bodies_ren, 
						click, scale, sys->len
					);

					if (new_fix_i != sys->len){
						flags->fix_i = new_fix_i;
						movement.x = 0;
						movement.y = 0;
					}
				}

				else if (event.button.button == 3){
					flags->fix_i = sys->len;
					movement.x = 0;
					movement.y = 0;
					shift.x = (float)vp.width / 2.0;
					shift.y = (float)vp.height / 2.0;
				}
			}
		}

		if (flags->fix_i < sys->len){
			shift.x = (float)vp.width / 2.0 - scale * sys->bodies[flags->fix_i].x + movement.x;
			shift.y = (float)vp.height / 2.0 - scale * sys->bodies[flags->fix_i].y + movement.y;
		}

		SDL_SetRenderDrawColor(vp.ren, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(vp.ren);
		SDL_GetRendererOutputSize(vp.ren, &vp.width, &vp.height);
		render_bodies(&vp, sys->bodies, sys->bodies_ren, shift, scale, sys->len);
		SDL_RenderPresent(vp.ren);

		if (flags->ispause){
			continue;
		}

		clrscr();
		printf("Iter: %u\n", sys->iter);
		log_bodies(sys->bodies, sys->len);

		update_coords(sys->bodies, sys->len, sys->G);

		elapsed = SDL_GetTicks() - start;

		if (elapsed < estimated){
			SDL_Delay(estimated - elapsed);
		}
	}

cleanup:
	if (sys){
		free(sys->bodies_ren);
		free(sys->bodies);
		free(sys);
	}
	
	vp_cleanup(&vp);
	free(flags->sys_filepath);
	free(flags);

	return 0;
}