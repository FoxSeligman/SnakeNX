#include <unistd.h>

#include <SDL.h>
#include <SDL_image.h>
#include <sys/time.h>

#include <switch.h>

#include "game.h"
#include "utility.h"

#define JOY_PLUS  10
#define JOY_LEFT  12
#define JOY_UP    13
#define JOY_RIGHT 14
#define JOY_DOWN  15

// Define the desired framebuffer resolution (here we set it to 720p).
#define SCREEN_W 1280
#define SCREEN_H 720

// Remove above and uncomment below for 1080p
//#define SCREEN_W  1920
//#define SCREEN_H 1080

#define GAME_COLUMNS 40
#define GAME_ROWS 22    // SCREEN_H / SCREEN_W * GAME_COLUMNS

#define CELL_SIZE SCREEN_W / GAME_COLUMNS
#define CELL_SIZE_HALF CELL_SIZE / 2

bool determineDirection(Direction* dir, Uint8 button)
{
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

void renderBlip(Game* game, SDL_Renderer* renderer, SDL_Texture* t_blip, Blip* blip)
{
    float blip_cell_y = game->progress * ((float)blip->target_row - blip->row) + blip->row;
    float blip_cell_x = game->progress * ((float)blip->target_column - blip->column) + blip->column;

    int blip_screen_y = (int)(SCREEN_H - blip_cell_y * CELL_SIZE);    // Flip vertically
    int blip_screen_x = (int)(blip_cell_x * CELL_SIZE);

    SDL_Rect rect = { blip_screen_x - CELL_SIZE_HALF, blip_screen_y - CELL_SIZE_HALF, CELL_SIZE, CELL_SIZE };
    SDL_RenderCopy(renderer, t_blip, NULL, &rect);
}

// TODO: Not implied that last_timestamp will update
void getDeltaTime(float* out_delta_time, struct timeval* last_timestamp) 
{
    struct timeval timestamp;
    gettimeofday(&timestamp, NULL);

    time_t deltaSeconds = timestamp.tv_sec - last_timestamp->tv_sec;
    suseconds_t deltaUsec = (timestamp.tv_usec - last_timestamp->tv_usec + (deltaSeconds * 1000000));
    
    *out_delta_time = deltaUsec / 1e6;
    *last_timestamp = timestamp;
}

SDL_Texture* loadImage(SDL_Renderer* renderer, const char* path) {
    SDL_Surface *surface = IMG_Load(path);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

// Main program entrypoint
int main(int argc, char** argv)
{
    romfsInit();
    chdir("romfs:/");

    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("Snake NX", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    SDL_Texture* t_bg = loadImage(renderer, "img/t_bg.png");
    SDL_Texture* t_blip = loadImage(renderer, "img/t_blip.png");

    
    // TODO: SDL_INIT_GAMECONTROLLER
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    SDL_JoystickEventState(SDL_ENABLE);
    SDL_JoystickOpen(0);

    // Create new game
    Game game;
    gameInit(&game, GAME_COLUMNS, GAME_ROWS, 0.05f);

    // Time
    struct timeval lastTimestamp;
    float deltaTime;
    getDeltaTime(&deltaTime, &lastTimestamp);   // Initialize values

    // Main loop
    SDL_Event event;
    bool exit_requested = false;
    while (appletMainLoop() && !exit_requested)
    {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                exit_requested = true;

            if (event.type == SDL_JOYBUTTONDOWN) {
                printf("%d\n",event.jbutton.button);
                if (event.jbutton.button == JOY_PLUS)
                    exit_requested = true;

                Direction nextDir;
                if (determineDirection(&nextDir, event.jbutton.button)) {
                    if (!game.has_started) {
                        gameStart(&game, nextDir);
                    } else {
                        gameSetNextDir(&game, nextDir);
                    }
                }
            }
        }

        // TODO: Can render just a part of it
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, t_bg, NULL, NULL);

        getDeltaTime(&deltaTime, &lastTimestamp);
        gameUpdate(&game, deltaTime);


        SDL_SetRenderDrawColor(renderer, 0, 127, 255, 255);
        
        for (Blip* blip = &game.root_blip; blip != NULL; blip = blip->next)
            renderBlip(&game, renderer, t_blip, blip);
        renderBlip(&game, renderer, t_blip, &game.loose_blip);

        SDL_RenderPresent(renderer);
    }

    gameCleanup(&game);

    IMG_Quit();
    SDL_Quit();
    romfsExit();

    return 0;
}
