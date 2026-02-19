//
// Created by zhenya on 19.02.2026.
//

#ifndef SDL_TEST_CONSOLEMANAGER_H
#define SDL_TEST_CONSOLEMANAGER_H
#include <string>
#include "SDL3_ttf/SDL_ttf.h"
#include "SDL3/SDL_render.h"


class GameManager;
class GameObject;

class ConsoleManager {
public:
    ConsoleManager(GameManager& gm);

    ~ConsoleManager();

    SDL_AppResult consoleLoop();

    SDL_AppResult loadFont(std::string fontPath, std::string fontName);

    SDL_AppResult consoleEventLoop(SDL_Event *event);

private:
    GameManager& game;
    SDL_Texture *gConsole = nullptr;
    std::string gConsoleBuffer;
    TTF_Font *gFont = nullptr;
};


#endif //SDL_TEST_CONSOLEMANAGER_H
