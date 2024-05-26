/**
 * @file uuid.c
 * 
 * @brief Very simple UUID generator for symbol tables.
 * 
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 2024-05-25
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "uuid.h"

static bool seeded = false;
static char buf[33];

/**
 * @brief Convert a buffer of bytes to a string.
 * 
 * @param buffer 
 * @return const char* 
 */
static const char* conv_strg(unsigned char buffer[16]) {

    char hexes[] = "0123456789ABCDEF";

    for(int i = 0; i < 16; i++)  {
        buf[i * 2] = hexes[(buffer[i] & 0xf0) >> 4];
        buf[i * 2 + 1] = hexes[buffer[i] & 0x0f];
    }
    buf[32] = 0;

    return (const char*)buf;
}

/**
 * @brief Generate a simple (and not compliant) UUID and return it as a 
 * string.
 * 
 * @return const char* 
 */
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

    return (const char*)conv_strg(buffer);
}

