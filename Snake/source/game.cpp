#include "game.h"

#include <stdlib.h>
#include <time.h>

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

void gameCheckEatBlip(Game* game)
{
    Blip* root_blip = &game->root_blip;
    Blip* loose_blip = &game->loose_blip;
    if (root_blip->column == loose_blip->column &&
        root_blip->row == loose_blip->row) {
            Blip* last_blip = getLastBlip(root_blip);

            // Make new blip and add to the end of the snake™
            Blip* new_blip = (Blip*)malloc(sizeof(Blip));
            blipInit(new_blip, last_blip->column, last_blip->row);
            last_blip->next = new_blip;
            
            gameMoveLooseBlip(game);
        }
}

void gameUpdate(Game* game, float delta_time)
{
    if (game->has_started) {
        float new_progress = game->progress + delta_time / game->settings.speed;
        if (new_progress >= 1) {
            blipSetNextTargets(&game->root_blip, game->next_dir);
            new_progress -= 1;

            gameCheckEatBlip(game);
        }
        
        game->progress = new_progress;
    }
}

void gameCleanup(Game* game)
{
    if (game->root_blip.next)
        blipDeleteRecursive(game->root_blip.next);
}
