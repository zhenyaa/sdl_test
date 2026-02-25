//
// Created by zhenya on 17.02.2026.
//

#include "GameManager.h"

#include "GameObject.h"
// #include "../cmake-build-debug/_deps/sdl3-src/src/video/SDL_sysvideo.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
GameManager::GameManager() {
}

GameManager::~GameManager() {
}

bool GameManager::init(const int window_width, const int window_height) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return false;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear",
                                   window_width,
                                   window_height,
                                   SDL_WINDOW_RESIZABLE,
                                   &window,
                                   &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return false;
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    // style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    // style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
    // ImGui_ImplSDLRenderer3_Init(renderer);
    // ImGui_ImplSDL3_InitForSDLRenderer(renderer, window);
    if (!SDL_SetRenderLogicalPresentation(renderer, window_width, window_height,
                                     SDL_LOGICAL_PRESENTATION_DISABLED)) {
        SDL_Log("Couldn't set logical presentation: %s", SDL_GetError());
        return false;
    }
    SDL_GetWindowSize(window, &width, &height);
    lastTick = SDL_GetTicks();
    return true;
}

void GameManager::handleSystemEvent(SDL_Event *event) {
    switch(event->type) {
        case SDL_EVENT_WINDOW_RESIZED:
            SDL_GetWindowSize(window, &width, &height);
            break;
        default: break;
    }
}

void GameManager::handleGameEvent(SDL_Event *event) {
    bool pressed = (event->type == SDL_EVENT_KEY_DOWN);
    switch(event->key.key) {
        case SDLK_LEFT: moveLeft = pressed; break;
        case SDLK_RIGHT: moveRight = pressed; break;
        case SDLK_SPACE: jumpPressed = pressed; break;
        default: break;
    }
}

void GameManager::render() {
    // SDL_SetRenderDrawColor(renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
    // SDL_RenderClear(renderer);
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    player->draw(renderer, true);
    for (auto& plat: platforms) {
        plat->draw(renderer, true);
    }
    // SDL_RenderPresent(renderer);
}

void GameManager::update() {
    while (accumulator >= GameConfig::fixed_dt) {
        float moveDir = 0.f;
        if (moveLeft)  moveDir -= 1.f;
        if (moveRight) moveDir += 1.f;

        if (player->rb) {
            player->rb->velocityX = moveDir * GameConfig::move_speed;
        }
        if (jumpPressed) {
            player->jump(GameConfig::jump_power);
            jumpPressed = false;
        }

        player->update(GameConfig::fixed_dt);
        accumulator -= GameConfig::fixed_dt;
    }

    // коллизии
    for (auto& platform : platforms) {
        if (player->collider.collisionDetection(player.get(), platform.get())) {
            player->onCollision(platform.get());
        }
    }

    // скроллинг, переделать на обьект камера
    float scrollThreshold = width / 3.0f;
    if (player->x - worldOffsetX > scrollThreshold) {
        worldOffsetX = player->x - scrollThreshold;
    }
}

void GameManager::updateTick(const Uint64 tick) {
    float frameDt = (tick - lastTick) / 1000.0f;
    lastTick = tick;
    if (frameDt > GameConfig::max_frame_dt) {
        frameDt = GameConfig::max_frame_dt;
    }
    accumulator += frameDt;
}

void GameManager::addObject(std::unique_ptr<GameObject> obj) {
    platforms.push_back(std::move(obj));
}

SDL_Renderer * GameManager::getRenderer() {
    return renderer;
}

SDL_Window * GameManager::getWindow() {
    return window;
}

int GameManager::getWidth() {
    int w;
    SDL_GetWindowSize(window, &w, nullptr);
    return w;
}

int GameManager::getHeight() {
    int h;
    SDL_GetWindowSize(window, nullptr, &h);
    return h;
}

void GameManager::shutdown() {
    SDL_Log("Shutting down...");
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
}

GameObject* GameManager::spawnPlayer(float x, float y) {
    player = std::make_unique<GameObject>(x,y);
    player->collider.boxes.push_back(
        {0.f, 0.f, 50.f, 50.f}
        );
    player->x = width / 2.0f;
    player->y = height / 2.0f;

    return player.get();
}

// bool GameManager::registerEvent(SDL_Event *event) {
//     SDL_Event
// };
