//
// Created by zhenya on 17.02.2026.
//

#ifndef SDL_TEST_PREFABPROCESSOR_H
#define SDL_TEST_PREFABPROCESSOR_H
#include <iostream>
#include <optional>
#include <unordered_map>

#include "Tileset.h"
#include <vector>

#include "Collider.h"
#include "SDL3/SDL_render.h"
// #include "../cmake-build-debug/_deps/sdl3-src/src/render/SDL_sysrender.h"

struct Prefab {
    std::string name;
    SDL_Texture* sprite;
    std::vector<BoxCollider> colliders;
    bool hasRigidBody = true;
};

class PrefabProcessor {
public:
    static std::optional<Prefab> get(const std::string& prefab_name);

    static bool loadFromFile(const std::string &path);

    // static void saveToFile(const std::string& path, const std::vector<Prefab>& prefabs);
private:
    static std::unordered_map<std::string, Prefab> prefabs;
};


#endif //SDL_TEST_PREFABPROCESSOR_H
