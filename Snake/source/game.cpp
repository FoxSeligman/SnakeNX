#include "game.hpp"

#include <stdlib.h>
#include <time.h>

#include <stdio.h>

// What the game should look like before the first move is made
Game::Game(u32 columns, u32 rows, float speed)
{
    this->has_started = false;
    
    this->settings.columns = columns;
    this->settings.rows = rows;
    this->settings.speed = speed;
    
    time_t t;
    srand((unsigned) time(&t));
    
    int x = this->settings.columns / 2;
    int y = this->settings.rows / 2;
    
    this->root_blip = this->last_blip = std::make_shared<Blip>(Cell{x, y});
    this->loose_blip = std::make_shared<Blip>(Cell{0, 0});

    this->set_progress(1);
}

// What the game should look like as soon as the first move is made
void Game::Start(Direction starting_dir)
{
    this->set_next_dir(starting_dir);

    MakeLooseBlip();

    this->has_started = true;
}

void Game::Update(float delta_time)
{

    if (this->has_started) {
        // TODO: Line too long
        float new_progress = this->get_progress() + delta_time / this->settings.speed;
        if (new_progress >= 1) {
            this->root_blip->setNextTargets(this->get_next_dir());
            new_progress -= 1;

            CheckEatBlip();
        }
        
        this->set_progress(new_progress);
    }
}

void Game::RequestNextDir(Direction next) {
    // If sum of cardinal directions is odd (is perpindicular)
    if ((this->get_next_dir() + next) % 2 == 1)
        this->set_next_dir(next);
}

void Game::CheckEatBlip()
{
    if (this->root_blip->cell == this->loose_blip->cell) {
        // Add loose blip to the end of the snake and make new loose blip
        this->last_blip->next = this->loose_blip;
        this->last_blip = this->loose_blip;

        MakeLooseBlip();
    }
}

void Game::MakeLooseBlip()
{
    int x = rand() % this->settings.columns;
    int y = rand() % this->settings.rows;
    this->loose_blip = std::make_shared<Blip>(Cell{x, y});
}
