#pragma once

#include <map>

#include <SDL_image.h>

namespace atmy {

typedef SDL_Texture Image;

class Resources {
public:
    static Image* GetImage(const char* path);
    static bool LoadImage(const char* path);
    
private:
    static std::map<const char*, Image*> images_;
};

}  // namespace atmy
