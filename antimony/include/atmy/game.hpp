#pragma once

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

    float last_time;
    bool exit_requested_ = false;
};

}  // namespace atmy
