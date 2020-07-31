#pragma once

#include <SDL.h>

namespace atmy {

class Game {
public:
    // DEBUG: Is this being called?
    Game() {}
    virtual ~Game() {}
    bool Update();
    void Quit();


private:
    virtual void OnFrame(float delta_seconds) = 0;

    // Controller input events
    Uint32 last_frame_timestamp_;
    bool exit_requested_ = false;
};

}  // namespace atmy
