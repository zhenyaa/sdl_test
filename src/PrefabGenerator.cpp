//
// Created by zhenya on 14.02.2026.
//

#include <iostream>
#include <memory>
#include <SDL3/SDL.h>
#include <vector>

#include "Tileset.h"

#include "SDL3_image/SDL_image.h"

struct BoxCollider {
    float ox, oy, w, h;
};
const char * basePath;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

const sprites::SpriteInfo *IterSprite(bool reset = false) {
    static int index = 0;

    if (reset) {
        index = 0; // сброс к началу
        return nullptr;
    }

    if (sprites::TILESET::sprites[index].name == nullptr) {
        return nullptr; // конец массива
    }

    return &sprites::TILESET::sprites[index++];

}

int main(int argc, char** argv) {
    basePath = SDL_GetBasePath();
    std::cout << std::endl << "generator created";
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }
    if (basePath == nullptr) {
        SDL_Log("Couldn't create SDL: %s", SDL_GetError());

        return 1;
    }
    SDL_Log("Base path: %s", basePath);
    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear",
                                     800,
                                     600,
                                     SDL_WINDOW_RESIZABLE,
                                     &window,
                                     &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
                                     }

    SDL_Surface *surface;
    surface = IMG_Load("assets/images/TILED_files/sprite_sheet2.png");
    if (!surface) {
        std::cerr << "IMG_Load failed: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    tryAgain:
    SDL_FRect srcRect;
    auto* BigGrassSpike = IterSprite();
    srcRect.x = BigGrassSpike->x;
    srcRect.y = BigGrassSpike->y;
    srcRect.w = BigGrassSpike->w;
    srcRect.h = BigGrassSpike->h;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    std::vector<BoxCollider> colliders;
    SDL_FRect dstRect = { 100.f, 100.f, srcRect.w, srcRect.h };
    int blockSize = 16;
    for (int by = 0; by < srcRect.h; by += blockSize) {
        for (int bx = 0; bx < srcRect.w; bx += blockSize) {
            bool hasOpaque = false;
            for (int y = 0; y < blockSize && by + y < srcRect.h && !hasOpaque; y++) {
                Uint8* row = static_cast<Uint8*>(surface->pixels) + (static_cast<int>(srcRect.y) + by + y) * surface->pitch;
                Uint32* pixelsRow = reinterpret_cast<Uint32*>(row);
                for (int x = 0; x < blockSize && bx + x < srcRect.w; x++) {
                    Uint32 pixel = pixelsRow[static_cast<int>(srcRect.x) + bx + x];
                    Uint8 r,g,b,a;
                    const SDL_PixelFormatDetails* fmt = SDL_GetPixelFormatDetails(surface->format);
                    SDL_GetRGBA(pixel, fmt, SDL_GetSurfacePalette(surface), &r,&g,&b,&a);
                    if (a > 0) { hasOpaque = true; break; }
                }
            }
            if (hasOpaque) {
                colliders.push_back({float(bx), float(by), float(blockSize), float(blockSize)});
            }
        }
    }
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // рисуем текстуру
        SDL_RenderTexture(renderer, texture, &srcRect, &dstRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 180);
        SDL_Log("Colliders q= %d", static_cast<int>(colliders.size()));
        for (auto& collider : colliders) {
            SDL_FRect r = { dstRect.x + collider.ox, dstRect.y + collider.oy, collider.w, collider.h };
            SDL_RenderRect(renderer, &r);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(1000); // ~60fps
        goto tryAgain;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}