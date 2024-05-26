#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "uuid.h"

static bool seeded = false;
static char buf[33];

static const char* conv_strg(unsigned char buffer[16]) {

    char hexes[] = "0123456789ABCDEF";

    for(int i = 0; i < 16; i++)  {
        buf[i * 2] = hexes[(buffer[i] & 0xf0) >> 4];
        buf[i * 2 + 1] = hexes[buffer[i] & 0x0f];
    }
    buf[32] = 0;

    return (const char*)buf;
}

const char* generate_uuid() {

    unsigned char buffer[16];

    if(!seeded) {
        srand(time(NULL));
        seeded = true;
    }

    for(int i = 0; i < (16 / 4); i++) {
        int rnd = rand();

        for(int j = 0; j < 4; j++) {
            buffer[i * 4 + j] = (0xff & rnd >> (8 * j));
        }
    }

    // set the version to 4
    buffer[6] = (buffer[6] & 0x0f) | 0x40;

    // set the variant to 1 (a)
    buffer[8] = (buffer[8] & 0x0f) | 0xa0;

    return (const char*)conv_strg(buffer);
}

