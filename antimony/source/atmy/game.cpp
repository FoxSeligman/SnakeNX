#include <atmy/game.hpp>

#include <SDL.h>

#include <switch.h>

#include <antimony>

namespace atmy {

bool Game::Update() {
    // Calculate delta time in seconds
    Uint32 now = SDL_GetTicks();
    float delta_time = (now - this->last_frame_timestamp_)/1000.0f;
    this->last_frame_timestamp_ = now;

    // Run child-defined update
    this->OnFrame(delta_time);

    return appletMainLoop() && !this->exit_requested_;
}

void Game::Quit() {
    this->exit_requested_ = true;
}

}  // namespace atmy
