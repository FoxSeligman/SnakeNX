#ifndef GAME_H
#define GAME_H

#include <memory>

#include <switch.h>

#include "blip.hpp"
#include "utility.h"

/// Game structure.
typedef struct Game {
    Settings settings;
    bool has_started;
    std::shared_ptr<Blip> root_blip, last_blip, loose_blip;
    float progress;
    Direction next_dir;
} Game;

void gameInit(Game* game, u32 columns, u32 rows, float speed);
void gameStart(Game* game, Direction next);
void gameSetNextDir(Game* game, Direction next);
void gameUpdate(Game* game, float delta_time);
void gameCleanup(Game* game);

#endif
