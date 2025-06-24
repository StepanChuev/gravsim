#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "system.h"
#define KEY_LEN 7
#define VAL_LEN 64

System *read_system_from_file(const char *filepath){
	char iskey = 1, iscomment = 0;
	char key[KEY_LEN], value[VAL_LEN];
	size_t sym_i = 0, body_i = 0;
	System *sys = (System *)malloc(sizeof(System));
	FILE *file = fopen(filepath, "r");

	if (!file || !sys){
		free(sys);
		fclose(file);

		return NULL;
	}

	sys->iter = 0;
	sys->len = 0;
	sys->G = 1;
	sys->bodies = NULL;
	sys->bodies_ren = NULL;

	for (int ch = 0; ch != EOF;){
		ch = fgetc(file);

		if (ch == '#')
			iscomment = 1;

		if (ch == '\n')
			iscomment = 0;

		if ((ch == '\n' || ch == EOF) && sym_i != 0){
			value[sym_i] = '\0';
			iskey = 1;

			if (!strcmp(key, "@iter"))
				sys->iter = strtoul(value, NULL, 0);
			
			else if (!strcmp(key, "@G"))
				sys->G = strtof(value, NULL);

			else if (!strcmp(key, "@len") && sys->len == 0){
				sys->len = strtoull(value, NULL, 0);
				sys->bodies = (Body *)malloc(sys->len * sizeof(Body));
				sys->bodies_ren = (Body_Ren *)malloc(sys->len * sizeof(Body_Ren));

				if (!sys->bodies || !sys->bodies_ren){
					free(sys->bodies);
					free(sys->bodies_ren);
					free(sys);
					fclose(file);

					return NULL;
				}
			}

			else if (sys->len == 0){
				free(sys);
				fclose(file);

				return NULL;
			}

			else if (!strcmp(key, "x"))
				sys->bodies[body_i].x = strtof(value, NULL);

			else if (!strcmp(key, "y"))
				sys->bodies[body_i].y = strtof(value, NULL);

			else if (!strcmp(key, "vx"))
				sys->bodies[body_i].vx = strtof(value, NULL);

			else if (!strcmp(key, "vy"))
				sys->bodies[body_i].vy = strtof(value, NULL);

			else if (!strcmp(key, "m"))
				sys->bodies[body_i].m = strtof(value, NULL);

			else if (!strcmp(key, "r"))
				sys->bodies_ren[body_i].radius = strtof(value, NULL);

			else if (!strcmp(key, "rgb") && sym_i == 6){
				sys->bodies_ren[body_i].r = convert_hex_char_to_int(value[0]) * 16 +
					convert_hex_char_to_int(value[1]);
				sys->bodies_ren[body_i].g = convert_hex_char_to_int(value[2]) * 16 +
					convert_hex_char_to_int(value[3]);
				sys->bodies_ren[body_i].b = convert_hex_char_to_int(value[4]) * 16 +
					convert_hex_char_to_int(value[5]);
			}

			sym_i = 0;
		}

		if (ch == ':'){
			key[sym_i] = '\0';
			sym_i = 0;
			iskey = 0;
			continue;
		}

		if (ch == ' ' || ch == '\t' || ch == '\n' || iscomment)
			continue;

		if (ch == '%' && body_i + 1 < sys->len){
			body_i++;

			continue;
		}

		if (iskey && sym_i + 2 < KEY_LEN)
			key[sym_i++] = (char)ch;

		else if (sym_i + 2 < VAL_LEN)
			value[sym_i++] = (char)ch;
	}

	fclose(file);

	return sys;
}

int write_system_to_file(System *sys, const char *filepath){
	FILE *file = fopen(filepath, "w");

	if (!file || !sys){
		fclose(file);

		return 0;
	}

	fprintf(file, "@iter: %u\n@len: %lu\n@G: %.9f\n\n", sys->iter, sys->len, sys->G);

	for (size_t i = 0; i < sys->len; i++){
		if (i != 0)
			fprintf(file, "%%\n");

		fprintf(
			file, 
			"x: %.9f\ny: %.9f\nvx: %.9f\nvy: %.9f\nm: %.9f\nr: %.9f\nrgb: %02X%02X%02X\n",
			sys->bodies[i].x, sys->bodies[i].y, sys->bodies[i].vx, sys->bodies[i].vy,
			sys->bodies[i].m, sys->bodies_ren[i].radius, sys->bodies_ren[i].r,
			sys->bodies_ren[i].g, sys->bodies_ren[i].b
		);
	}

	return 1;
}

int convert_hex_char_to_int(char ch){
	if (ch >= '0' && ch <= '9')
		return ch - '0';

	if (ch >= 'A' && ch <= 'F')
		return ch - 'A' + 10;

	if (ch >= 'a' && ch <= 'f')
		return ch - 'a' + 10;

	return -1;
}