//
// Created by zhenya on 17.02.2026.
//

#ifndef SDL_TEST_PREFABPROCESSOR_H
#define SDL_TEST_PREFABPROCESSOR_H
#include <iostream>

class PrefabProcessor {
public:
    struct Prefab {
        std::string name;
        sprites::SpriteInfo sprite;
        std::vector<BoxCollider> colliders;
        bool hasRigidBody = true;
    };
    static get(const std::string prefab_name);

    // загрузить JSON-файл с префабами
    static std::vector<Prefab> loadFromFile(const std::string& path);

    // сохранить префабы в файл
    static void saveToFile(const std::string& path, const std::vector<Prefab>& prefabs);
};


#endif //SDL_TEST_PREFABPROCESSOR_H