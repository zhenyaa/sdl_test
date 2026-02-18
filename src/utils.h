//
// Created by zhenya on 06.02.2026.
//

#ifndef SDL_TEST_UTILS_H
#define SDL_TEST_UTILS_H
#include <string>
#include <nlohmann/json.hpp>

#include "SDL3/SDL_render.h"
#include "SDL3_image/SDL_image.h"

namespace FileUtils {
    // std::optional<nlohmann::json> loadJSON(const std::string& path, const std::string &sprite_name);
    std::optional<nlohmann::json> loadJSON(const std::string& path);
    std::string readTextFile(const std::string& path);
    // void saveJSON(const std::string& path, const nlohmann::json& j);
}

namespace TextureUtils {
    using TexturePtr = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;

    class TextureManager {
    public:
        static SDL_Texture* get(const std::string& name);
        static bool loadSurface(const std::string& path);
        static bool processTextures(SDL_Renderer* renderer);

    private:
        static std::unique_ptr<SDL_Surface, decltype(&SDL_DestroySurface)> surface;
        static std::unordered_map<std::string, TexturePtr> textures;
    };
}
#endif //SDL_TEST_UTILS_H