//
// Created by zhenya on 06.02.2026.
//

#ifndef SDL_TEST_GAMEOBJECT_H
#define SDL_TEST_GAMEOBJECT_H
#include <cstdint>
#include <memory>

#include "config.h"
#include "RigidBody.h"
#include "SDL3/SDL_render.h"


class GameObject {
private:
    int D;
    bool moving = false;

public:
    int x;
    int y;
    int height;
    int width;
    std::unique_ptr<RigidBody> rb;

    GameObject(int x, int y) : x(x), y(y), width(10), height(10), D(10) {
        rb = std::make_unique<RigidBody>(this, GameConfig::gravity);
    };

    void update(float dt);

    void draw(SDL_Renderer *renderer);

    void jump(float power= 400.f);
};


#endif //SDL_TEST_GAMEOBJECT_H
