#include <atmy/game.hpp>

#include <SDL.h>

#include <switch.h>

#include <antimony>

namespace atmy {

bool Game::Update() {
    // Calculate delta time in seconds
    Uint32 thisTime = SDL_GetTicks();
    float deltaTime = (thisTime - this->last_time)/1000.0f;
    this->last_time = thisTime;

    // Run child-defined update
    this->OnFrame(deltaTime);

    return appletMainLoop() && !this->exit_requested_;
}

void Game::Quit() {
    this->exit_requested_ = true;
}

}  // namespace atmy
