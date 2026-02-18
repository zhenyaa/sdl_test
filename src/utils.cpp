//
// Created by zhenya on 06.02.2026.
//
#include "utils.h"
#include <fstream>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_init.h>
#include <optional>
#include "Tileset.h"
#include <nlohmann/json.hpp>
namespace FileUtils {
    std::optional<nlohmann::json> loadJSON(const std::string &path, const std::string &sprite_name) {
        std::ifstream file(path);
        SDL_Log("CWD: %s",
                std::filesystem::current_path().c_str());
        if (!file) {
            SDL_Log("Failed to open prefabs.json");
            return std::nullopt;
        }
        nlohmann::json j = nlohmann::json::parse(file);
        file.close();
        if (!j.contains(sprite_name)) {
            SDL_Log("No prefab for tile: %s", sprite_name.c_str());
            return std::nullopt;
        }

        if (!j[sprite_name].contains("colliders")) {
            SDL_Log("No colliders for tile: %s", sprite_name.c_str());
            return std::nullopt;
        }
        return j[sprite_name];
    }

    std::optional<nlohmann::json> loadJSON(const std::string &path) {
        std::ifstream file(path);
        if (!file) {
            SDL_Log("Failed to open prefabs.json");
            return std::nullopt;
        }
        nlohmann::json j = nlohmann::json::parse(file);
        // if (!j) {
        //     SDL_Log("Failed to parse %s", path.c_str());
        //     return std::nullopt;
        // }
        SDL_Log("Loading %zu prefabs", j.size());
        file.close();
        return j;
    }
}

namespace TextureUtils {
    std::unique_ptr<SDL_Surface, decltype(&SDL_DestroySurface)> TextureManager::surface{nullptr, SDL_DestroySurface};
    std::unordered_map<std::string, TexturePtr> TextureManager::textures;

    SDL_Texture *TextureManager::get(const std::string& name) {
        auto it = textures.find(name);

        if (it == textures.end()) {
            SDL_Log("Texture not found: %s", name.c_str());
            return nullptr;
        }

        return it->second.get();
    }

    bool TextureManager::loadSurface(const std::string &path) {
        SDL_Surface* raw = SDL_LoadSurface(path.c_str());
        if (!raw) {
            SDL_Log("Failed to load surface %s: %s", path.c_str(), SDL_GetError());
            return false;
        }
        surface = std::unique_ptr<SDL_Surface, decltype(&SDL_DestroySurface)>(raw, SDL_DestroySurface);
        return true;
    }

    bool TextureManager::processTextures(SDL_Renderer *renderer) {
        if (!surface) {
            return false;
        }
        int sprite_data_index = 0;
        while (sprites::TILESET::sprites[sprite_data_index].name != nullptr) {
            auto& currentSprite = sprites::TILESET::sprites[sprite_data_index];
            const SDL_FRect rect = {
                static_cast<float>(currentSprite.x),
                static_cast<float>(currentSprite.y),
                static_cast<float>(currentSprite.w),
                static_cast<float>(currentSprite.h)
            };
            SDL_Texture* tex = SDL_CreateTexture(renderer,
                                                 SDL_PIXELFORMAT_RGBA8888,
                                                 SDL_TEXTUREACCESS_TARGET,
                                                 rect.w, rect.h);
            if (!tex) {
                SDL_Log("Failed to create texture: %s", SDL_GetError());
                return false;
            }
            SDL_SetRenderTarget(renderer, tex);
            SDL_Texture* surfaceTex = SDL_CreateTextureFromSurface(renderer, surface.get());
            SDL_RenderTexture(renderer, surfaceTex, &rect, nullptr);
            SDL_DestroyTexture(surfaceTex);
            SDL_SetRenderTarget(renderer, nullptr);
            textures.emplace(currentSprite.name, TexturePtr(tex, SDL_DestroyTexture));

            sprite_data_index++;
        }
        return true;
    }
}

