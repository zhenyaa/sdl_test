//
// Created by zhenya on 17.02.2026.
//

#ifndef SDL_TEST_GAMEOBJECTFACTORY_H
#define SDL_TEST_GAMEOBJECTFACTORY_H
#include "iostream"

class GameManager;
class GameObject;

class GameObjectFactory {
public:
    bool spawn(
        GameManager *gm,
        const std::string &prefab,
        float x, float y
    );
};


#endif //SDL_TEST_GAMEOBJECTFACTORY_H
