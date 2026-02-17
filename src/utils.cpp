//
// Created by zhenya on 06.02.2026.
//
#include "utils.h"
#include <fstream>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_init.h>
#include <optional>

std::optional<nlohmann::json> FileUtils::loadJSON(const std::string &path, const std::string &sprite_name) {

    // std::ifstream file("./prefubs.json");
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
