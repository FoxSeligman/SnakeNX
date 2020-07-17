#include <atmy/resources.hpp>

#include <SDL_image.h>

#include <atmy/system.hpp>

namespace atmy {

std::map<const char*, Image*> Resources::images_;

Image* Resources::GetImage(const char* path) {
    // Lazy loading
    if (Resources::images_.count(path) == 0)
        LoadImage(path);

    return Resources::images_[path];
}

bool Resources::LoadImage(const char* path) {
    SDL_Renderer* renderer = System::get_renderer();
    // TODO: Throw exception: System/renderer hasn't been initialized
    if (!renderer)
        return false;

    // TODO: Throw exception: Image with this path has already been loaded 
    if (Resources::images_.count(path) == 1)
        return false;

    SDL_Surface* surface = IMG_Load(path);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    Resources::images_[path] = texture;

    return true;
}

}  // namespace atmy
