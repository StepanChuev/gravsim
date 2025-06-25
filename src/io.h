#ifndef IO_H
#define IO_H

#include "body.h"

#ifdef _WIN32
#include <conio.h>
#else
#define clrscr() printf("\e[1;1H\e[2J")
#endif

typedef struct Flags {
	char ispause;
	char isdraw;
	char isuse_ren_info;
	char islog;
	char islogi;
	char islogb;
	int width;
	int height;
	int fps;
	float scale_step;
	float move_step;
	size_t fix_i;
	size_t save_mult_iter;
	size_t exit_iter;
	char *sys_filepath;
	char *save_dir;
	char *save_pref;
} Flags;

Flags *get_flags_from_args(int argc, char **argv, Flags *default_flags);
void log_bodies(Body *bodies, size_t len);

#endif