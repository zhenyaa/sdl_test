//
// Created by zhenya on 19.02.2026.
//

#include "ConsoleManager.h"

#include "GameManager.h"
#include "GameObject.h"

ConsoleManager::ConsoleManager(GameManager& gm) : game(gm) {
    SDL_Log("Creating Console");
    TTF_Init();
    if (!gFont) {
        gFont = TTF_OpenFont("assets/fonts/Consolas-Regular.ttf", 18);
        if (!gFont) {
            SDL_Log("Failed to load font: %s", SDL_GetError());
            return;
        }
    }
    gConsole =
            SDL_CreateTexture(
                game.getRenderer(),
                SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET,
                game.getWidth(),
                game.getHeight() / 3
            );

    if (!gConsole) {
        SDL_Log("cant create texture");
        return;
    }
    SDL_Renderer *renderer = game.getRenderer();
    SDL_SetRenderTarget(renderer, gConsole);
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_FRect dstRect = {0, 0, (float) game.getWidth(), (float) game.getHeight() / 3};
    SDL_RenderTexture(renderer, gConsole, nullptr, &dstRect);
    SDL_RenderPresent(renderer);
    SDL_StartTextInput(game.getWindow());
}

ConsoleManager::~ConsoleManager() {
    SDL_StopTextInput(game.getWindow());
    if (gConsole)
        SDL_DestroyTexture(gConsole);
    if (gFont)
        TTF_CloseFont(gFont);
}


SDL_AppResult ConsoleManager::consoleLoop() {
    if (!gConsole || !gFont) return SDL_APP_FAILURE;

    SDL_Renderer* renderer = game.getRenderer();
    SDL_SetRenderTarget(renderer, gConsole);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 128);
    SDL_RenderClear(renderer);

    if (!gConsoleBuffer.empty()) {
        SDL_Color color{255, 255, 255, 255};
        SDL_Surface* surf = TTF_RenderText_Blended(gFont, gConsoleBuffer.c_str(), gConsoleBuffer.size(), color);
        if (surf) {
            SDL_Texture* txt = SDL_CreateTextureFromSurface(renderer, surf);
            SDL_FRect dst{5, 5, (float)surf->w, (float)surf->h};
            SDL_RenderTexture(renderer, txt, nullptr, &dst);
            SDL_DestroyTexture(txt);
            SDL_DestroySurface(surf);
        }
    }

    SDL_SetRenderTarget(renderer, nullptr);

    SDL_FRect dstRect = {0,0,(float)game.getWidth(), (float)game.getHeight()/3};
    SDL_RenderTexture(renderer, gConsole, nullptr, &dstRect);
    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}


SDL_AppResult ConsoleManager::consoleEventLoop(SDL_Event *event) {
    switch (event->type)
    {
        case SDL_EVENT_TEXT_INPUT:
            gConsoleBuffer += event->text.text;
            break;

        case SDL_EVENT_KEY_DOWN:
        {
            SDL_Keycode key = event->key.key;
            if (key == SDLK_BACKSPACE && !gConsoleBuffer.empty())
                gConsoleBuffer.pop_back();
            if (key == SDLK_RETURN)
            {
                SDL_Log("Command: %s", gConsoleBuffer.c_str());
                gConsoleBuffer.clear();
            }
            if (key == SDLK_ESCAPE)
            {
                game.console = nullptr; //is it normal
            }
            break;
        }
    }

    return SDL_APP_CONTINUE;
}
