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
#include <sstream>
#include "utils.h"


struct GameManager {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    int width = 0;
    int height = 0;
    GameObject player{1, 1};
    std::vector<std::unique_ptr<GameObject>> platforms;
    TexturePtr bgGradient;
    TexturePtr mainTex;
    float worldOffsetX = 0.0f;
    Uint64 lastTick = 0;
    float accumulator = 0.0f;
    bool moveLeft = false;
    bool moveRight = false;
    bool jumpPressed = false;
};

GameObject* AddPlatform(GameManager* gm, float x, float y, const Sprites::Tile& tile) {
    auto obj = std::make_unique<GameObject>(x, y);
    std::string path;
    std::stringstream ss;
    ss << "assets/images/TILED_files/" << tile.textureFile;
    path = ss.str();
    obj->sprite.texture = IMG_LoadTexture(gm->renderer, path.c_str()); // SDL_Texture*
    obj->sprite.src = tile.src;
    gm->platforms.push_back(std::move(obj));
    return gm->platforms.back().get();
}

static int SDLCALL TestThread(void *ptr) {
    int cnt;

    for (cnt = 0; cnt < 10; ++cnt) {
        // SDL_Log("Thread counter: %d", cnt);
        SDL_Delay(1000);
    }

    return cnt;
}

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
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());

        return SDL_APP_FAILURE;
    }


    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear",
                                     GameConfig::window_width,
                                     GameConfig::window_height,
                                     SDL_WINDOW_RESIZABLE,
                                     &state->window,
                                     &state->renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(state->renderer, GameConfig::window_width, GameConfig::window_height,
                                     SDL_LOGICAL_PRESENTATION_DISABLED);

    SDL_GetWindowSize(state->window, &state->width, &state->height);
    state->player.x = state->width / 2.0f;
    state->player.y = state->height / 2.0f;
    std::cout << "w/h " << state->width << ":" << state->height << std::endl;
    const char *n = SDL_GetRendererName(state->renderer);
    std::cout << "Renderer name: " << n << std::endl;
    state->bgGradient = CreateGradient(state->renderer, state->width, state->height);
    GameObject* gm1 = AddPlatform(state.get(), 0, (float)state->height-40, Sprites::Objects_82);
    GameObject* gm2 = AddPlatform(state.get(), 20, (float)state->height-40, Sprites::Objects_82);
    GameObject* gm3 = AddPlatform(state.get(), 40, (float)state->height-40, Sprites::Objects_82);
    state->mainTex.reset(IMG_LoadTexture(state->renderer, "assets/images/main_asset.png"));
    if (!state->mainTex) {
        SDL_Log("Failed to load image: %s", SDL_GetError());
    } else {
        SDL_Log("Image loaded successfully!");
    }

    state->lastTick = SDL_GetTicks();
    *appstate = state.release();
    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    auto *state = static_cast<GameManager*>(appstate);
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS; // end the program
    }
    if (event->type == SDL_EVENT_WINDOW_RESIZED) {
        SDL_GetWindowSize(state->window, &state->width, &state->height);
    }
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.key) {
            case SDLK_SPACE:
                state->jumpPressed = true;
                std::cout << "press key space " << state->player.y << std::endl;
                return SDL_APP_CONTINUE;
            case SDLK_LEFT:
                std::cout << "press key " << state->player.y << std::endl;
                state->moveLeft = true;
                return SDL_APP_CONTINUE;
            case SDLK_RIGHT:
                std::cout << "press key " << state->player.y << std::endl;
                state->moveRight = true;
                return SDL_APP_CONTINUE;
            default:
                return SDL_APP_CONTINUE;
        }
    }
    if (event->type == SDL_EVENT_KEY_UP) {
        switch (event->key.key) {
            case SDLK_LEFT:
                state->moveLeft = false;
                return SDL_APP_CONTINUE;
            case SDLK_RIGHT:
                state->moveRight = false;
                return SDL_APP_CONTINUE;
            default:
                return SDL_APP_CONTINUE;
        }
    }

    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {
    auto *state = static_cast<GameManager*>(appstate);
    Uint64 now = SDL_GetTicks();
    float frameDt = (now - state->lastTick) / 1000.0f;
    state->lastTick = now;
    if (frameDt > GameConfig::max_frame_dt) {
        frameDt = GameConfig::max_frame_dt;
    }
    state->accumulator += frameDt;

    while (state->accumulator >= GameConfig::fixed_dt) {
        float moveDir = 0.0f;
        if (state->moveLeft) moveDir -= 1.0f;
        if (state->moveRight) moveDir += 1.0f;
        if (state->player.rb) {
            state->player.rb->velocityX = moveDir * GameConfig::move_speed;
        }
        if (state->jumpPressed) {
            state->player.jump(GameConfig::jump_power);
            state->jumpPressed = false;
        }
        state->player.update(GameConfig::fixed_dt);
        state->accumulator -= GameConfig::fixed_dt;
    }

    float scrollThreshold = state->width / 3.0f;
    if (state->player.x - state->worldOffsetX > scrollThreshold) {
        state->worldOffsetX = state->player.x - scrollThreshold;
    }

    SDL_RenderTexture(state->renderer, state->bgGradient.get(), NULL, NULL);
    for (auto& plat:state->platforms) {
        plat->draw(state->renderer);
    }
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
    state->player.draw(state->renderer);

    SDL_RenderPresent(state->renderer);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    std::unique_ptr<GameManager> state{static_cast<GameManager*>(appstate)};
    if (state) {
        if (state->renderer) {
            SDL_DestroyRenderer(state->renderer);
        }
        if (state->window) {
            SDL_DestroyWindow(state->window);
        }
    }
    std::cout << "quit" << std::endl;
    /* SDL will clean up the window/renderer for us. */
}
