#include "blip.h"

#include <stdlib.h>

void blipInit(Blip* blip, u32 column, u32 row)
{
    blip->next = NULL;
    blip->column = blip->target_column = column;
    blip->row = blip->target_row = row;
}

void blipSetNextTargetsRecursive(Blip* blip, u32 target_column, u32 target_row)
{
    blip->column = blip->target_column;
    blip->row = blip->target_row;

    blip->target_column = target_column;
    blip->target_row = target_row;

    if (blip->next)
        blipSetNextTargetsRecursive(blip->next, blip->column, blip->row);
}

void blipSetNextTargets(Blip* blip, Direction next)
{
    u32 target_column = blip->target_column;
    u32 target_row = blip->target_row;

    switch(next) {
    case DIR_LEFT:
        target_column -= 1;
        break;
    case DIR_UP:
        target_row += 1;
        break;
    case DIR_RIGHT:
        target_column += 1;
        break;
    case DIR_DOWN:
    default:
        target_row -= 1;
    }

    blipSetNextTargetsRecursive(blip, target_column, target_row);
}

void blipDeleteRecursive(Blip* blip)
{
    if (blip->next)
        blipDeleteRecursive(blip->next);

    free(blip);
}