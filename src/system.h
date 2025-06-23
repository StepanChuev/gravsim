#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>
#include "body.h"
#include "bodyren.h"

typedef struct System {
	uint32_t iter;
	float G;
	size_t len;
	Body *bodies;
	Body_Ren *bodies_ren;
} System;

System *parse_system_from_file(const char *filepath);
int convert_hex_char_to_int(char ch);

#endif