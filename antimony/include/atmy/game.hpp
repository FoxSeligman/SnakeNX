#pragma once

#include <map>

#include <SDL.h>

#include <atmy/controller.hpp>

namespace atmy {

class Game {
public:
    // There are a maximum of 8 players, and they're all populated upon app 
    // creation. Valid values range from 0 to 7 inclusive.
    typedef int PlayerIndex;

    Game() {}
    virtual ~Game() {}
    bool Update();
    void Quit();

    // void AssignSwitchControllerToPlayer(int controller_index, 
    //                                     PlayerIndex p_index);
    bool RegisterController(atmy::Controller* controller,
                            PlayerIndex p_index);

protected:
    virtual void OnFrame(float delta_seconds) = 0;

private:
    Uint32 last_frame_timestamp_;
    bool exit_requested_ = false;

    // The PLAYER index remains constant whether the physical controller is 
    // connected or not. It's determined by the Switch itself.
    std::map<PlayerIndex, atmy::Controller*> controllers_;
};

}  // namespace atmy
