#include <atmy/system.hpp>

#include <unistd.h>

#include <SDL_image.h>

#include <switch.h>

// TODO: Window needs to adjust automatically depending on whether the Switch
// is docked or not
#define SCREEN_W 1280
#define SCREEN_H 720

namespace atmy {

SDL_Window* System::window_;
SDL_Renderer* System::renderer_;

// TODO: Use parameters
void System::Init() {
    // Don't Init twice
    if (System::window_) return;

    romfsInit();
    chdir("romfs:/");

    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
    IMG_Init(IMG_INIT_PNG);

    System::window_ = SDL_CreateWindow("Snake NX", SDL_WINDOWPOS_CENTERED, 
                                    SDL_WINDOWPOS_CENTERED, 
                                    SCREEN_W, SCREEN_H, 
                                    SDL_WINDOW_SHOWN);
    System::renderer_ = SDL_CreateRenderer(System::window_, -1, SDL_RENDERER_SOFTWARE);
}

void System::Terminate() {
    IMG_Quit();
    SDL_Quit();
    romfsExit();
}

}  // namespace atmy
