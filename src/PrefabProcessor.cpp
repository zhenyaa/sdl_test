//
// Created by zhenya on 17.02.2026.
//

#include "PrefabProcessor.h"
#include <optional>
#include "utils.h"
#include "SDL3/SDL_log.h"

std::unordered_map<std::string, Prefab> PrefabProcessor::prefabs;

std::optional<Prefab> PrefabProcessor::get(const std::string& prefab_name) {
    if (prefab_name.empty()) {
        return std::nullopt;
    }
    if (prefabs.empty()) {
        return std::nullopt;
    }
    return prefabs.at(prefab_name);
}

bool PrefabProcessor::loadFromFile(const std::string &path) {
    prefabs.clear();

    auto jsonObj = FileUtils::loadJSON(path);
    if (!jsonObj) {
        SDL_Log("Failed to load prefab file: %s", path.c_str());
        return false;
    }
    try {
        for (auto& [key, val] : jsonObj->items()) {

            // if (!val.contains("name") || !val["name"].is_string()) {
            //     SDL_Log("Prefab without valid name, skipped");
            //     continue;
            // }

            std::string name = key;

            // Берём готовую текстуру
            SDL_Texture* tex = TextureUtils::TextureManager::get(name);
            if (!tex) {
                SDL_Log("Texture not found for prefab: %s", name.c_str());
                continue;
            }

            Prefab p;
            p.name = name;
            p.sprite = tex;

            try {
                if (val.contains("colliders")) {
                    for (auto& c : val["colliders"]) {
                        p.colliders.push_back({
                            c.value("x", 0.f),
                            c.value("y", 0.f),
                            c.value("w", 0.f),
                            c.value("h", 0.f)
                        });
                    }
                }
            } catch (const nlohmann::json::type_error& e) {
                SDL_Log("Warning: Invalid 'rigidbody' field for prefab '%s': %s",
                name.c_str(), e.what());
                p.hasRigidBody = true; // fallback to default
            }
            // Rigidbody
             p.hasRigidBody = true;

            prefabs.emplace(name, std::move(p));
        }
    } catch (const nlohmann::json::type_error& e) {
        SDL_Log("Failed to load prefab: %s", e.what());
    }
    SDL_Log("Loaded %zu prefabs", prefabs.size());
    return true;
};
