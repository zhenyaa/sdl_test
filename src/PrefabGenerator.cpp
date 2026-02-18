//
// Created by zhenya on 14.02.2026.
//

#include <iostream>
#include <memory>
#include <SDL3/SDL.h>
#include <vector>
#include <ostream>
#include <nlohmann/json.hpp>
#include "Tileset.h"
#include <fstream>
#include "config.h"
#include "SDL3_image/SDL_image.h"

struct BoxCollider {
    float ox, oy, w, h;
};
const char * basePath;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

struct CellOfPixels {
    int x, y;
    int step;
    int r, g, b, a;
};

CellOfPixels processPixelsToCell(
    SDL_Surface* surface,
    const SDL_PixelFormatDetails* fmt,
    const SDL_FRect& srcRect,
    int blockSize,
    int bx,
    int by)
{
    uint64_t sumR = 0;
    uint64_t sumG = 0;
    uint64_t sumB = 0;
    uint64_t sumA = 0;

    int count = 0;

    for (int y = 0; y < blockSize; y++) {
        for (int x = 0; x < blockSize; x++) {

            int px = static_cast<int>(srcRect.x) + bx + x;
            int py = static_cast<int>(srcRect.y) + by + y;

            Uint32* pixel = (Uint32*)(
                (Uint8*)surface->pixels +
                py * surface->pitch +
                px * fmt->bytes_per_pixel
            );

            Uint8 r, g, b, a;

            SDL_GetRGBA(
                *pixel,
                fmt,
                SDL_GetSurfacePalette(surface),
                &r, &g, &b, &a
            );

            sumR += r;
            sumG += g;
            sumB += b;
            sumA += a;

            count++;
        }
    }

    Uint8 avgR = sumR / count;
    Uint8 avgG = sumG / count;
    Uint8 avgB = sumB / count;
    Uint8 avgA = sumA / count;

    return CellOfPixels(
        bx,        // позиция блока
        by,
        blockSize,
        avgR,
        avgG,
        avgB,
        avgA
    );
}

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
    nlohmann::json j;

    tryAgain:
    SDL_FRect srcRect;
    auto* sprite_meta = IterSprite();

    if (sprite_meta == nullptr) {
        auto path = SDL_GetBasePath();
        auto fullPath = std::string(path) + GameConfig::EVN_PREFAB_PATH;
        std::ofstream file(fullPath);
        file << j;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        file.close();
        SDL_Quit();
        return 0;
    } // exit block
    srcRect.x = sprite_meta->x;
    srcRect.y = sprite_meta->y;
    srcRect.w = sprite_meta->w;
    srcRect.h = sprite_meta->h;
    SDL_FRect dstRect = { 100.f, 100.f, srcRect.w, srcRect.h };
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    std::vector<BoxCollider> colliders;
    int blockSize = 16;
    const SDL_PixelFormatDetails* fmt = SDL_GetPixelFormatDetails(texture->format);
    for (int blockY=0; blockY < static_cast<int>(srcRect.h); blockY += blockSize) { // iterate by vertical line with step in blockSize
        for (int blockX=0; blockX < static_cast<int>(srcRect.w); blockX += blockSize) { //iterate by evry cell by horizont with step in blockSize
            SDL_Log("vertical: %d, horizont %d", blockY, blockX);
            auto block = processPixelsToCell(surface, fmt, srcRect, blockSize, blockX, blockY);
            SDL_Log("block r: %d, g: %d, b: %d, a: %d", block.r, block.g, block.b, block.a);
            if (block.a > 80) {
                colliders.push_back({static_cast<float>(block.x), static_cast<float>(block.y), static_cast<float>(blockSize) * dstRect.w / srcRect.w, static_cast<float>(blockSize) * dstRect.h / srcRect.h});
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


        SDL_RenderTexture(renderer, texture, &srcRect, &dstRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 180);
        // SDL_Log("Colliders q= %d", static_cast<int>(colliders.size()));
        nlohmann::json collidersJson = nlohmann::json::array();
        for (auto& collider : colliders) {
            SDL_FRect r = { dstRect.x + collider.ox, dstRect.y + collider.oy, collider.w, collider.h };
            collidersJson.push_back({
                  {"x", collider.ox},
                  {"y", collider.oy},
                  {"w", collider.w},
                  {"h", collider.h}
              });
            SDL_RenderRect(renderer, &r);

        }
        nlohmann::json spriteSize = nlohmann::json::array();
        spriteSize.push_back(srcRect.w);
        spriteSize.push_back(srcRect.h);
        j[sprite_meta->name]["size"] = spriteSize;
        j[sprite_meta->name]["colliders"] = collidersJson;
        SDL_Log("data json %s",j.dump(2).c_str());
        SDL_RenderPresent(renderer);
        SDL_Delay(1000); // ~60fps
        goto tryAgain;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}