#pragma once

#include <memory>
#include <SDL3/SDL.h>

struct SDLTextureDeleter {
    void operator()(SDL_Texture* texture) const noexcept {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }
};

using TexturePtr = std::unique_ptr<SDL_Texture, SDLTextureDeleter>;
