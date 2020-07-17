#pragma once

#include <SDL.h>

namespace atmy {

class System {
public:
    static void Init();
    static void Terminate();
    static SDL_Renderer* get_renderer() {
        return renderer_;
    }
private:
    static SDL_Window* window_;
    static SDL_Renderer* renderer_;
};

}  // namespace atmy
