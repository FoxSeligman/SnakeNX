#pragma once

#include <memory>

#include <switch.h>

#include <antimony>

#include "blip.hpp"
#include "utility.h"

// TODO: Indents should be 2 spaces
class SnakeGame : public atmy::Game {
public:
    SnakeGame(u32 columns, u32 rows, float speed);
    void Start(Direction next);
    void RequestNextDir(Direction next);
    
    // Accessors
    inline float get_progress() { return progress_; }
    inline void set_progress(float value) { progress_ = value; }
    inline Direction get_next_dir() { return next_dir_; }
    
    Settings settings;
    bool has_started;
    std::shared_ptr<Blip> root_blip, last_blip, loose_blip;

private:
    void OnFrame(float delta_seconds) override;

    void CheckEatBlip();
    void MakeLooseBlip();
    inline void set_next_dir(Direction next) { next_dir_ = next; }

    float progress_;
    Direction next_dir_;
};
