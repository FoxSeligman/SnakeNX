#include "snake_game.hpp"

#include <stdlib.h>
#include <time.h>

#include <SDL.h>

#include <stdio.h>

// TODO
#define SCREEN_W 1280
#define SCREEN_H 720

// Remove above and uncomment below for 1080p
//#define SCREEN_W  1920
//#define SCREEN_H 1080

#define GAME_COLUMNS 40
#define GAME_ROWS 22    // SCREEN_H / SCREEN_W * GAME_COLUMNS

#define CELL_SIZE SCREEN_W / GAME_COLUMNS
#define CELL_SIZE_HALF CELL_SIZE / 2

#define JOY_PLUS  10
#define JOY_LEFT  12
#define JOY_UP    13
#define JOY_RIGHT 14
#define JOY_DOWN  15

// What the game should look like before the first move is made
SnakeGame::SnakeGame(u32 columns, u32 rows, float speed) {
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


    // TODO: SDL_INIT_GAMECONTROLLER
    // SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    SDL_InitSubSystem(SDL_INIT_EVERYTHING);
    // SDL_JoystickEventState(SDL_ENABLE);
    SDL_GameControllerEventState(SDL_ENABLE);
    // SDL_JoystickOpen(0);
    SDL_GameController* ctrl0 = SDL_GameControllerOpen(0);
    SDL_Joystick* joy0 = SDL_GameControllerGetJoystick(ctrl0);
    SDL_GameControllerSetPlayerIndex(ctrl0, 0);
    SDL_GameController* ctrl1 = SDL_GameControllerOpen(1);
    SDL_Joystick* joy1 = SDL_GameControllerGetJoystick(ctrl1);
    SDL_GameControllerSetPlayerIndex(ctrl1, 1);

    printf("Controller 0: %d\n", SDL_JoystickGetDeviceVendor(0));
    printf("Controller 1: %d\n", SDL_JoystickGetDeviceVendor(1));
}

// What the game should look like as soon as the first move is made
void SnakeGame::Start(Direction starting_dir) {
    this->set_next_dir(starting_dir);

    MakeLooseBlip();

    this->has_started = true;
}

// TODO
bool determineDirection(Direction* dir, Uint8 button) {
    switch (button) {
        case JOY_LEFT:
            *dir = DIR_LEFT;
            break;
        case JOY_UP:
            *dir = DIR_UP;
            break;
        case JOY_RIGHT:
            *dir = DIR_RIGHT;
            break;
        case JOY_DOWN:
            *dir = DIR_DOWN;
           break;
        default:
            return false;
    }
    return true;
}

void renderBlip(SnakeGame* game, SDL_Renderer* renderer, SDL_Texture* t_blip, Blip* blip) {
    float blip_cell_y = game->get_progress() * ((float)blip->target_cell.row - blip->cell.row) + blip->cell.row;
    float blip_cell_x = game->get_progress() * ((float)blip->target_cell.column - blip->cell.column) + blip->cell.column;

    int blip_screen_y = (int)(SCREEN_H - blip_cell_y * CELL_SIZE);    // Flip vertically
    int blip_screen_x = (int)(blip_cell_x * CELL_SIZE);

    SDL_Rect rect = { blip_screen_x - CELL_SIZE_HALF, blip_screen_y - CELL_SIZE_HALF, CELL_SIZE, CELL_SIZE };
    SDL_RenderCopy(renderer, t_blip, NULL, &rect);
}

void SnakeGame::OnFrame(float delta_seconds) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        printf("%#05x\n", event.type);

        if (event.type == SDL_QUIT)
            this->Quit();

        if (event.type == SDL_JOYBUTTONDOWN) {
            printf("Controller %d: %d\n",event.jbutton.which, event.jbutton.button);
            if (event.jbutton.button == JOY_PLUS)
                this->Quit();
            Direction nextDir;
            if (determineDirection(&nextDir, event.jbutton.button)) {
                if (!this->has_started) {
                    this->Start(nextDir);
                } else {
                    this->RequestNextDir(nextDir);
                }
            }
        }

        // if (event.type == SDL_CONTROLLERDEVICEADDED) {
        //     printf("It was %d\n", event.cdevice.which);
        // }
    }

    // TODO: Can render just a part of it
    SDL_Renderer* renderer = atmy::System::get_renderer();
    SDL_Texture* t_bg = atmy::Resources::GetImage("img/t_bg.png");
    SDL_Texture* t_blip = atmy::Resources::GetImage("img/t_blip.png");

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, t_bg, NULL, NULL);

    if (this->has_started) {
        // TODO: Line too long
        float new_progress = this->get_progress() + delta_seconds / this->settings.speed;
        if (new_progress >= 1) {
            this->root_blip->setNextTargets(this->get_next_dir());
            new_progress -= 1;

            CheckEatBlip();
        }
        
        this->set_progress(new_progress);
    }


    SDL_SetRenderDrawColor(renderer, 0, 127, 255, 255);
    
    for (auto blip = this->root_blip.get(); blip != nullptr; blip = blip->next.get())
        renderBlip(this, renderer, t_blip, blip);
    renderBlip(this, renderer, t_blip, this->loose_blip.get());

    SDL_RenderPresent(renderer);
}

void SnakeGame::RequestNextDir(Direction next) {
    // If sum of cardinal directions is odd (is perpindicular)
    if ((this->get_next_dir() + next) % 2 == 1)
        this->set_next_dir(next);
}

void SnakeGame::CheckEatBlip() {
    if (this->root_blip->cell == this->loose_blip->cell) {
        // Add loose blip to the end of the snake and make new loose blip
        this->last_blip->next = this->loose_blip;
        this->last_blip = this->loose_blip;

        MakeLooseBlip();
    }
}

void SnakeGame::MakeLooseBlip() {
    int x = rand() % this->settings.columns;
    int y = rand() % this->settings.rows;
    this->loose_blip = std::make_shared<Blip>(Cell{x, y});
}
