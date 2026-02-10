//
// Created by zhenya on 06.02.2026.
//

#include "GameObject.h"

#include <iostream>

#include "SDL3/SDL_timer.h"

// GameObject::GameObject(int x, int y) : x(x), y(y){
// };

void GameObject::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColorFloat(renderer, 156.f / 255.f, 35.f / 255.f, 26.f / 255.f, 1.f);
    SDL_FRect obj_rect = {(float)x, (float)y, (float)width, (float)height};
    SDL_RenderFillRect(renderer, &obj_rect);
}

void GameObject::update() {
    Uint64 newTime = SDL_GetTicks();
    float dt = (newTime - lastTick) / 1000.0f;
    lastTick = newTime;
    if (rb) rb->update(dt);
}

void GameObject::jump(float power) {
    std::cout << "jumping" << std::endl;
    rb->velY = -power;
    std::cout << "velocity: " << rb->velY << std::endl;

}
