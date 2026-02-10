//
// Created by zhenya on 06.02.2026.
//

#include "GameObject.h"

#include <iostream>
#include <SDL3_image/SDL_image.h>

// GameObject::GameObject(int x, int y) : x(x), y(y){
// };

void GameObject::draw(SDL_Renderer* renderer) {
    if (!sprite.texture) {
        SDL_SetRenderDrawColorFloat(renderer, 156.f / 255.f, 35.f / 255.f, 26.f / 255.f, 1.f);
        SDL_FRect obj_rect = {(float)x, (float)y, (float)width, (float)height};
        SDL_RenderFillRect(renderer, &obj_rect);
        return;
    }

    SDL_FRect dstRect = {
        (float)x,
        (float)y,
        sprite.src.w,   // используем размер тайла
        sprite.src.h
    };
    SDL_RenderTexture(renderer, sprite.texture, &sprite.src, &dstRect);
}

void GameObject::update(float dt) {
    if (rb) rb->update(dt);
}

void GameObject::jump(float power) {
    std::cout << "jumping" << std::endl;
    rb->velocityY = -power;
    std::cout << "velocity: " << rb->velocityY << std::endl;
}

bool GameObject::loadSprite(SDL_Renderer* renderer, const char* path, SDL_FRect srcRect) {
    SDL_Texture* tex = IMG_LoadTexture(renderer, path);
    if (!tex) {
        SDL_Log("Failed to load texture '%s': %s", path, SDL_GetError());
        sprite.texture = nullptr; // fallback
        sprite.src = srcRect;
        width = (int)srcRect.w;
        height = (int)srcRect.h;
        return false;
    }
    sprite.texture = tex;
    sprite.src = srcRect;
    width = (int)srcRect.w;
    height = (int)srcRect.h;
    return true;
}

