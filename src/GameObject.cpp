//
// Created by zhenya on 06.02.2026.
//

#include "GameObject.h"

#include <iostream>

// GameObject::GameObject(int x, int y) : x(x), y(y){
// };

void GameObject::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColorFloat(renderer, 156.f / 255.f, 35.f / 255.f, 26.f / 255.f, 1.f);
    SDL_FRect obj_rect = {(float)x, (float)y, (float)width, (float)height};
    SDL_RenderFillRect(renderer, &obj_rect);
}

void GameObject::update(float dt) {
    if (rb) rb->update(dt);
}

void GameObject::jump(float power) {
    std::cout << "jumping" << std::endl;
    rb->velocityY = -power;
    std::cout << "velocity: " << rb->velocityY << std::endl;

}
