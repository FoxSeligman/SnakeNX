#include "game.h"

#include <stdlib.h>
#include <time.h>

#include <stdio.h>

void gameMakeLooseBlip(Game* game)
{
    int x = rand() % game->settings.columns;
    int y = rand() % game->settings.rows;
    game->loose_blip = std::make_shared<Blip>(Cell{x, y});
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
    
    int x = game->settings.columns / 2;
    int y = game->settings.rows / 2;
    
    game->root_blip = game->last_blip = std::make_shared<Blip>(Cell{x, y});
    game->loose_blip = std::make_shared<Blip>(Cell{0, 0});

    game->progress = 1;
}

// What the game should look like as soon as the first move is made
void gameStart(Game* game, Direction starting_dir)
{
    game->next_dir = starting_dir;

    gameMakeLooseBlip(game);

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
    if (game->root_blip->cell == game->loose_blip->cell) {
        // Add loose blip to the end of the snake and make new loose blip
        game->last_blip->next = game->loose_blip;
        game->last_blip = game->loose_blip;

        gameMakeLooseBlip(game);
    }
}

void gameUpdate(Game* game, float delta_time)
{

    if (game->has_started) {
        // TODO: Line too long
        float new_progress = game->progress + delta_time / game->settings.speed;
        if (new_progress >= 1) {
            game->root_blip->setNextTargets(game->next_dir);
            new_progress -= 1;

            gameCheckEatBlip(game);
        }
        
        game->progress = new_progress;
    }
}

void gameCleanup(Game* game)
{
    
}
