#ifndef LSC_H
#define LSC_H

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define ENV_SIZE (sizeof(char) * 4096)
#define LINE_SIZE (sizeof(char) * 256)

struct Lsc_Info
{
    char* env;
    char* line;
};

struct Lsc_Info* lsc_getenv();

// Initialize LSC
void lsc_init();

void lsc_load(const char* statement);

void lsc_resolve();

#endif