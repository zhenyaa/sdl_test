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
#include "Collider.h"

struct Sprite {
    SDL_Texture* texture = nullptr;
    SDL_FRect src{};
};

class GameObject {
private:
    float scale = 1.0f;
    bool moving = false;

public:
    int x;
    int y;
    int height;
    int width;
    std::unique_ptr<RigidBody> rb;
    Sprite sprite;
    Collider collider;

    GameObject(int x, int y) : x(x), y(y), width(10), height(10) {
        rb = std::make_unique<RigidBody>(this, GameConfig::gravity);
    };
    GameObject(int x, int y, float scale) : x(x), y(y), width(10), height(10), scale(scale) {
        rb = std::make_unique<RigidBody>(this, GameConfig::gravity);
    };

    // GameObject(int x, int y) : x(x), y(y), width(10), height(10), D(10) {
    //     rb = std::make_unique<RigidBody>(this, GameConfig::gravity);
    // }; need add several types to have objects without rb and other

    void update(float dt);

    void draw(SDL_Renderer *renderer);
    void draw(SDL_Renderer *renderer, bool debug);

    void jump(float power= 400.f);
    bool loadSprite(SDL_Renderer* renderer, const char* path, SDL_FRect srcRect);
    void onCollision(GameObject* collider);
};


#endif //SDL_TEST_GAMEOBJECT_H
