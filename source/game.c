#include "game.h"

#include <stdlib.h>

void blipInit(Blip* blip, u32 column, u32 row)
{
    blip->next = NULL;
    blip->column = blip->target_column = column;
    blip->row = blip->target_row = row;
}

// What the game should look like before the first move is made
void gameInit(Game* game, u32 columns, u32 rows, float speed)
{
    game->has_started = false;
    
    game->settings.columns = columns;
    game->settings.rows = rows;
    game->settings.speed = speed;
    
    blipInit(&game->root_blip, game->settings.columns / 2, game->settings.rows / 2);
    game->progress = 1;
}

// What the game should look like as soon as the first move is made
void gameStart(Game* game, Direction starting_dir)
{
    game->next_dir = starting_dir;

    game->has_started = true;
}

void gameSetNextDir(Game* game, Direction next)
{
    // If sum of cardinal directions is odd (is perpindicular)
    if ((game->next_dir + next) % 2 == 1)
        game->next_dir = next;
}

void blipSetNextTargetsRecursive(Blip* blip, u32 target_column, u32 target_row)
{
    if (blip->next)
        blipSetNextTargetsRecursive(blip->next, blip->column, blip->row);

    blip->column = blip->target_column;
    blip->row = blip->target_row;

    blip->target_column = target_column;
    blip->target_row = target_row;
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
        blip->target_row = blip->row - 1;
    }

    blipSetNextTargetsRecursive(blip, target_column, target_row);
}

void blipDeleteRecursive(Blip* blip)
{
    if (blip->next)
        blipDeleteRecursive(blip->next);

    free(blip);
}

void gameUpdate(Game* game, float delta_time)
{
    if (game->has_started) {
        float new_progress = game->progress + delta_time / game->settings.speed;
        if (new_progress >= 1) {
            blipSetNextTargets(&game->root_blip, game->next_dir);
            new_progress -= 1;
        }
        
        game->progress = new_progress;
    }
}

void gameCleanup(Game* game)
{
    if (game->root_blip.next)
        blipDeleteRecursive(game->root_blip.next);
}
