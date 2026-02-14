//
// Created by zhenya on 14.02.2026.
//

#ifndef SDL_TEST_PREFABPROCCESSOR_H
#define SDL_TEST_PREFABPROCCESSOR_H
#include <nlohmann/json.hpp>
#include "Collider.h"
#include "Tileset.h"

class PrefabProccessor {
public:
    struct Prefab {
        std::string name;
        sprites::SpriteInfo sprite;
        std::vector<BoxCollider> colliders;
        bool hasRigidBody = true;
    };

    // загрузить JSON-файл с префабами
    static std::vector<Prefab> loadFromFile(const std::string& path);

    // сохранить префабы в файл
    static void saveToFile(const std::string& path, const std::vector<Prefab>& prefabs);
};


#endif //SDL_TEST_PREFABPROCCESSOR_H