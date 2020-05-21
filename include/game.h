#ifndef GAME_H
#define GAME_H

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

/// Game structure.
typedef struct Game {
    Settings settings;
    bool has_started;
    Blip root_blip;
    Blip loose_blip;
    float progress;
    Direction next_dir;
} Game;

void gameInit(Game* game, u32 columns, u32 rows, float speed);
void gameStart(Game* game, Direction next);
void gameSetNextDir(Game* game, Direction next);
void gameUpdate(Game* game, float delta_time);
void gameCleanup(Game* game);

#endif
