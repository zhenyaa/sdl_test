//
// Created by zhenya on 06.02.2026.
//

#include "GameObject.h"

#include <iostream>
#include <SDL3_image/SDL_image.h>


void GameObject::draw(SDL_Renderer* renderer) {
    if (!sprite.texture) {
        SDL_SetRenderDrawColorFloat(renderer, 156.f / 255.f, 35.f / 255.f, 26.f / 255.f, 1.f);
        const SDL_FRect obj_rect = {static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height)};
        SDL_RenderFillRect(renderer, &obj_rect);
        return;
    }

    const SDL_FRect dstRect = {
        static_cast<float>(x),
        static_cast<float>(y),
        sprite.src.w * scale,
        sprite.src.h * scale
    };
    SDL_RenderTexture(renderer, sprite.texture, &sprite.src, &dstRect);
}

void GameObject::draw(SDL_Renderer *renderer, bool debug) {
    if (!sprite.texture) {
        SDL_SetRenderDrawColorFloat(renderer, 156.f/255.f, 35.f/255.f, 26.f/255.f, 1.f);
        const SDL_FRect obj_rect = {static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height)};
        SDL_RenderFillRect(renderer, &obj_rect);
    } else {
        const SDL_FRect dstRect = {static_cast<float>(x), static_cast<float>(y), sprite.src.w * scale, sprite.src.h * scale};
        SDL_RenderTexture(renderer, sprite.texture, nullptr, &dstRect);
    }

    if (debug) {
        std::cout<<"try render collider" << std::endl;
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180); // зелёный

        for (auto&[ox, oy, w, h] : collider.boxes) {
            SDL_FRect r = { x + ox, y + oy, w, h };
            SDL_RenderRect(renderer, &r);
        }
    }
}

void GameObject::update(float dt) {
    if (rb) rb->update(dt);
}

void GameObject::jump(float power) {
    if (!rb->onGround) return;
    rb->onGround = false;
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
        width = static_cast<int>(srcRect.w);
        height = static_cast<int>(srcRect.h);
        return false;
    }
    sprite.texture = tex;
    sprite.src = srcRect;
    width = static_cast<int>(srcRect.w);
    height = static_cast<int>(srcRect.h);
    return true;
}

void GameObject::onCollision(GameObject *otherObj) {
    SDL_Log("Collision detected");
    rb->onGround = true;
}

