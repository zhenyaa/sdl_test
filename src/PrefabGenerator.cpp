//
// Created by zhenya on 14.02.2026.
//

#include <iostream>
#include <SDL3/SDL.h>
#include <vector>
struct BoxCollider {
    float ox, oy, w, h;
};

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int main(int argc, char** argv) {
    std::cout << std::endl << "generator created";
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }
    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear",
                                     800,
                                     600,
                                     SDL_WINDOW_RESIZABLE,
                                     &window,
                                     &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
                                     }

    // Пример спрайта (можно загрузить через SDL_Image)
    SDL_FRect spriteRect = {100, 100, 64, 64};
    std::vector<BoxCollider> colliders = {
        {0, 0, 64, 10},
        {10, 10, 44, 54}
    };

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

        // отрисовка спрайта (здесь просто прямоугольник)
        SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255);
        SDL_RenderFillRect(renderer, &spriteRect);

        // отрисовка коллайдеров
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for (auto& box : colliders) {
            SDL_FRect r = {spriteRect.x + box.ox, spriteRect.y + box.oy, box.w, box.h};
            SDL_RenderFillRect(renderer, &r);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60fps
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}