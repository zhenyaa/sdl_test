#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <iostream>
#include <math.h>
#include <ostream>
#include <memory>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include "GameObject.h"
#include "config.h"
#include "sdl_raii.h"
#include <vector>
#include "generated_sprites.h"
#include "Tileset.h"
#include <sstream>
#include "utils.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "GameManager.h"
#include "PrefabProcessor.h"

// GameObject* AddPlatform(GameManager* gm, float x, float y, const sprites::SpriteInfo* tile, const std::vector<BoxCollider>& boxes)
// {
//     auto obj = std::make_unique<GameObject>(x, y, 2.f, boxes);
//     const std::string SPRITE_PATH = "assets/images/TILED_files/sprite_sheet2.png";
//     obj->sprite.texture = IMG_LoadTexture(gm->renderer, SPRITE_PATH.c_str());
//     if (!obj->sprite.texture)
//     {
//         SDL_Log("Failed to load texture: %s", SDL_GetError());
//         return nullptr;
//     }
//     obj->sprite.src = {
//         static_cast<float>(tile->x),
//         static_cast<float>(tile->y),
//         static_cast<float>(tile->w),
//         static_cast<float>(tile->h)
//     };
//
//     gm->platforms.push_back(std::move(obj));
//
//     return gm->platforms.back().get();
// }

// static int SDLCALL TestThread(void *ptr) {
//     int cnt;
//
//     for (cnt = 0; cnt < 10; ++cnt) {
//         // SDL_Log("Thread counter: %d", cnt);
//         SDL_Delay(1000);
//     }
//
//     return cnt;
// }

TexturePtr CreateGradient(SDL_Renderer *r, int w, int h) {
    SDL_Texture *raw = SDL_CreateTexture(
        r,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        w, h
    );
    TexturePtr tex(raw);
    SDL_SetRenderTarget(r, tex.get());

    for (int y = 0; y < h; y++) {
        float t = (float) y / (h - 1);
        float f = sinf(t * M_PI_2);

        Uint8 g = (Uint8) (f * 255);

        SDL_SetRenderDrawColor(r, 0, g, 255, 255);
        SDL_RenderLine(r, 0, y, w, y);
    }

    SDL_SetRenderTarget(r, NULL);
    return tex;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");
    auto state = std::make_unique<GameManager>();
    if (!state->init(GameConfig::window_width, GameConfig::window_height)) {
        return SDL_APP_FAILURE;
    }
    const std::string SPRITE_PATH = "assets/images/TILED_files/sprite_sheet2.png";
    TextureUtils::TextureManager::loadSurface(SPRITE_PATH);
    TextureUtils::TextureManager::processTextures(state->getRenderer());
    auto f = FileUtils::loadJSON("./prefubs.json");
    PrefabProcessor::loadFromFile("./prefubs.json");
    // блок генерации игрового обьекта
    Prefab pref = PrefabProcessor::get("HoleLarge").value();
    std::unique_ptr<GameObject> gras = std::make_unique<GameObject>(900, 700, 1.5f, pref.colliders);
    Sprite s = Sprite(pref.sprite, SDL_FRect{0,0,160,158}, true); // матод SDL_RenderTexture не отрабатывает без источника так как выходной считаеться по нему
    gras->sprite = s;
    state->addTexture(std::move(gras));
    // конец блок генерации игрового обьекта, решить с размерами обьектов

    state->spawnPlayer(1,1);

    *appstate = state.release();
    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    auto *state = static_cast<GameManager*>(appstate);
    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;

        case SDL_EVENT_WINDOW_RESIZED:
            state->handleSystemEvent(event);
            break;

        default:
            break;
    }
    if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_KEY_UP) {
        state->handleGameEvent(event);
    }

    return SDL_APP_CONTINUE;
    // if (event->type == SDL_EVENT_QUIT) {
    //     return SDL_APP_SUCCESS; // end the program
    // }
    // if (event->type == SDL_EVENT_WINDOW_RESIZED) {
    //     state->handleEvent(event);
    //     // SDL_GetWindowSize(state->window, &state->width, &state->height);
    // }
    // if (event->type == SDL_EVENT_KEY_DOWN) {
    //     state->handleEvent(event);
    //     // switch (event->key.key) {
    //     //     case SDLK_SPACE:
    //     //         state->jumpPressed = true;
    //     //         std::cout << "press key space " << state->player->y << std::endl;
    //     //         return SDL_APP_CONTINUE;
    //     //     case SDLK_LEFT:
    //     //         std::cout << "press key " << state->player->y << std::endl;
    //     //         state->moveLeft = true;
    //     //         return SDL_APP_CONTINUE;
    //     //     case SDLK_RIGHT:
    //     //         std::cout << "press key " << state->player->y << std::endl;
    //     //         state->moveRight = true;
    //     //         return SDL_APP_CONTINUE;
    //     //     default:
    //     //         return SDL_APP_CONTINUE;
    //     // }
    // }
    // if (event->type == SDL_EVENT_KEY_UP) {
    //     state->handleEvent(event);
    //     // switch (event->key.key) {
    //     //     case SDLK_LEFT:
    //     //         state->moveLeft = false;
    //     //         return SDL_APP_CONTINUE;
    //     //     case SDLK_RIGHT:
    //     //         state->moveRight = false;
    //     //         return SDL_APP_CONTINUE;
    //     //     default:
    //     //         return SDL_APP_CONTINUE;
    //     // }
    // }
    //
    // return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {
    auto *state = static_cast<GameManager*>(appstate);
    if (!state) {
        SDL_Log("cant get GameManager");
    }
    state->updateTick(SDL_GetTicks());
    state->update();
    state->render();
    // Uint64 now = SDL_GetTicks();
    // float frameDt = (now - state->lastTick) / 1000.0f;
    // state->lastTick = now;
    // if (frameDt > GameConfig::max_frame_dt) {
    //     frameDt = GameConfig::max_frame_dt;
    // }
    // state->accumulator += frameDt;
    //
    // while (state->accumulator >= GameConfig::fixed_dt) {
    //     float moveDir = 0.0f;
    //     if (state->moveLeft) moveDir -= 1.0f;
    //     if (state->moveRight) moveDir += 1.0f;
    //     if (state->player->rb) {
    //         state->player->rb->velocityX = moveDir * GameConfig::move_speed;
    //     }
    //     if (state->jumpPressed) {
    //         state->player->jump(GameConfig::jump_power);
    //         state->jumpPressed = false;
    //     }
    //     state->player->update(GameConfig::fixed_dt);
    //     state->accumulator -= GameConfig::fixed_dt;
    // }
    //
    // for (auto& platform : state->platforms) {
    //     if (state->player->collider.collisionDetection(state->player.get(), platform.get())) {
    //         state->player->onCollision(platform.get());
    //     }
    // }
    //
    // float scrollThreshold = state->width / 3.0f;
    // if (state->player->x - state->worldOffsetX > scrollThreshold) {
    //     state->worldOffsetX = state->player->x - scrollThreshold;
    // }
    //
    // SDL_RenderTexture(state->renderer, state->bgGradient.get(), NULL, NULL);
    // for (auto& plat:state->platforms) {
    //     plat->draw(state->renderer, true);
    // }
    // int numBlocks = (int) ceil(state->width / dst.w) + 1; // сколько блоков нужно
    // int firstBlockIndex = (int) (state->worldOffsetX / blockWidth);
    // for (int i = 0; i < numBlocks; i++) {
    //     SDL_FRect block;
    //     block.x = (firstBlockIndex + i) * blockWidth - state->worldOffsetX;
    //     block.y = state->height - blockHeight;
    //     block.w = blockWidth;
    //     block.h = blockHeight;
    //     if (block.x + block.w >= 0 && block.x <= state->width) {
    //         SDL_RenderTexture(state->renderer, state->mainTex.get(), &src, &block);
    //     }
    // }
    // state->player->draw(state->renderer, true);
    //
    // SDL_RenderPresent(state->renderer);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    std::unique_ptr<GameManager> state{static_cast<GameManager*>(appstate)};
    state->shutdown();
    std::cout << "quit" << std::endl;
    /* SDL will clean up the window/renderer for us. */
}
