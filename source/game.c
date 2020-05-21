#include "game.h"

#include <stdlib.h>
#include <time.h>

void blipInit(Blip* blip, u32 column, u32 row)
{
    blip->next = NULL;
    blip->column = blip->target_column = column;
    blip->row = blip->target_row = row;
}

void gameMoveLooseBlip(Game* game)
{
    blipInit(&game->loose_blip, rand() % game->settings.columns, rand() % game->settings.rows);
}

// What the game should look like before the first move is made
void gameInit(Game* game, u32 columns, u32 rows, float speed)
{
    game->has_started = false;
    
    game->settings.columns = columns;
    game->settings.rows = rows;
    game->settings.speed = speed;
    
    time_t t;
    srand((unsigned) time(&t));
    
    blipInit(&game->root_blip, game->settings.columns / 2, game->settings.rows / 2);
    blipInit(&game->loose_blip, 0, 0);
    game->progress = 1;
}

Blip* getLastBlip(Blip* blip)
{
    if (blip == NULL)
        return NULL;

    Blip* last_blip = blip;
    while (last_blip->next != NULL)
        last_blip = last_blip->next;

    return last_blip;
}

// What the game should look like as soon as the first move is made
void gameStart(Game* game, Direction starting_dir)
{
    game->next_dir = starting_dir;

    gameMoveLooseBlip(game);

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

void gameCheckEatBlip(Game* game)
{
    Blip* root_blip = &game->root_blip;
    Blip* loose_blip = &game->loose_blip;
    if (root_blip->target_column == loose_blip->column &&
        root_blip->target_row == loose_blip->row) {
            Blip* new_blip = (Blip*)malloc(sizeof(Blip));
            blipInit(new_blip, loose_blip->column, loose_blip->row);

            getLastBlip(root_blip)->next = new_blip;
            gameMoveLooseBlip(game);
        }
}

void gameUpdate(Game* game, float delta_time)
{
    if (game->has_started) {
        float new_progress = game->progress + delta_time / game->settings.speed;
        if (new_progress >= 1) {
            gameCheckEatBlip(game);
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
