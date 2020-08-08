#include <atmy/game.hpp>

#include <switch.h>

#include <antimony>

namespace atmy {

bool Game::Update() {
    // Calculate delta time in seconds
    Uint32 now = SDL_GetTicks();
    float delta_time = (now - last_frame_timestamp_)/1000.0f;
    last_frame_timestamp_ = now;

    hidScanInput();

    // If CONTROLLER_P1_AUTO has a Controller registered to respond to it
    // TODO: Loop through map, if map isn't empty
    if (controllers_.find(CONTROLLER_P1_AUTO) != controllers_.end()) {
        u64 keys_down = hidKeysDown(CONTROLLER_P1_AUTO);
        u64 keys_held = hidKeysHeld(CONTROLLER_P1_AUTO);
        if (keys_down != 0)
            controllers_[CONTROLLER_P1_AUTO]->OnButtonsDown(keys_down);
    }
    

    // Run child-defined update
    this->OnFrame(delta_time);

    return appletMainLoop() && !this->exit_requested_;
}

void Game::Quit() {
    this->exit_requested_ = true;
}

bool Game::RegisterController(atmy::Controller* controller,
                              PlayerIndex p_index) {
    // If a controller has already been registered with this id
    if (controllers_.find(p_index) != controllers_.end())
        return false;

    controllers_[p_index] = controller;
    return true;
}

}  // namespace atmy
