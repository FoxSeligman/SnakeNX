#ifndef BLIP_H
#define BLIP_H

#include <switch.h>

#include "utility.h"

/// Blip structure.
typedef struct Blip {
    u32 target_column;
    u32 target_row;
    u32 column;
    u32 row;
    struct Blip* next;
} Blip;

void blipInit(Blip* blip, u32 column, u32 row);
void blipSetNextTargets(Blip* blip, Direction next);
void blipDeleteRecursive(Blip* blip);

#endif
