#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <iostream>
#include <math.h>
#include <ostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include "GameObject.h"
#include "utils.h"

// #include "../cmake-build-debug/_deps/sdl3-src/src/render/SDL_sysrender.h"

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Rect *rects = NULL;
int width, height;
GameObject player(1, 1);
const int D = 2;
SDL_Thread *thread;
int threadReturnValue;
SDL_Texture *bgGradient = nullptr;
SDL_Texture *mainTex = nullptr;
float worldOffsetX = 0.0f;


static int SDLCALL TestThread(void *ptr) {
    int cnt;

    for (cnt = 0; cnt < 10; ++cnt) {
        // SDL_Log("Thread counter: %d", cnt);
        SDL_Delay(1000);
    }

    return cnt;
}

SDL_Texture *CreateGradient(SDL_Renderer *r, int w, int h) {
    SDL_Texture *tex = SDL_CreateTexture(
        r,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        w, h
    );
    SDL_SetRenderTarget(r, tex);

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

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());

        return SDL_APP_FAILURE;
    }


    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, 640, 480, SDL_LOGICAL_PRESENTATION_DISABLED);

    SDL_GetWindowSize(window, &width, &height);
    player.x = width / 2.0f;
    player.y = height / 2.0f;
    std::cout << "w/h " << width << ":" << height << std::endl;
    const char *n = SDL_GetRendererName(renderer);
    std::cout << "Renderer name: " << n << std::endl;
    bgGradient = CreateGradient(renderer, width, height);
    mainTex = IMG_LoadTexture(renderer, "assets/images/main_asset.png");
    if (!mainTex) {
        SDL_Log("Failed to load image: %s", SDL_GetError());
    } else {
        SDL_Log("Image loaded successfully!");
    }

    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS; // end the program
    }
    if (event->type == SDL_EVENT_WINDOW_RESIZED) {
        SDL_GetWindowSize(window, &width, &height);
    }
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.key) {
            case SDLK_SPACE:
                player.jump(1000.f);
                std::cout << "press key space " << player.y << std::endl;
                return SDL_APP_CONTINUE;
            case SDLK_UP:
                if (player.y > 1) player.y -= 10;
                std::cout << "press key " << player.y << std::endl;
                return SDL_APP_CONTINUE;
            case SDLK_DOWN:
                std::cout << "press key " << player.y << std::endl;
                player.y += 10;
                return SDL_APP_CONTINUE;
            case SDLK_LEFT:
                std::cout << "press key " << player.y << std::endl;
                if (player.x > 1) player.x -= 10;
                return SDL_APP_CONTINUE;
            case SDLK_RIGHT:
                std::cout << "press key " << player.y << std::endl;
                if (player.x <= width / 2) {
                    player.x += 10;
                }
                worldOffsetX += 10;
                return SDL_APP_CONTINUE;
            default:
                return SDL_APP_CONTINUE;
        }
    }

    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {
    float scrollThreshold = width / 3.0f;
    if (player.x - worldOffsetX > scrollThreshold) {
        worldOffsetX = player.x - scrollThreshold;
    }

    SDL_FRect src; // part of the texture
    src.x = 0; // x in the PNG
    src.y = 16; // y in the PNG
    src.w = 15; // width of the sprite
    src.h = 15; // height of the sprite

    SDL_FRect dst;
    dst.x = 100; // screen X
    dst.y = 50; // screen Y
    dst.w = 200; // width to draw
    dst.h = 150; // height to draw
    float blockWidth = dst.w;
    float blockHeight = dst.h;
    SDL_RenderTexture(renderer, bgGradient, NULL, NULL);

    int numBlocks = (int) ceil(width / dst.w) + 1; // сколько блоков нужно
    int firstBlockIndex = (int) (worldOffsetX / blockWidth);
    for (int i = 0; i < numBlocks; i++) {
        SDL_FRect block;
        block.x = (firstBlockIndex + i) * blockWidth - worldOffsetX;
        block.y = height - blockHeight;
        block.w = blockWidth;
        block.h = blockHeight;
        if (block.x + block.w >= 0 && block.x <= width) {
            SDL_RenderTexture(renderer, mainTex, &src, &block);
        }
    }
    player.update();
    // std::cout << "player x: " << player.x << std::endl;
    // std::cout << "player y: " << player.y << std::endl;
    // std::cout << "player fravity: " << player.rb->gravity << std::endl;
    // std::cout << "player vel" << player.rb->velY << std::endl;
    player.draw(renderer);

    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    if (bgGradient)
        SDL_DestroyTexture(bgGradient);
    if (mainTex)
        SDL_DestroyTexture(mainTex);
    std::cout << "quit" << std::endl;
    /* SDL will clean up the window/renderer for us. */
}
