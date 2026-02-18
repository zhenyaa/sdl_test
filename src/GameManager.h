//
// Created by zhenya on 17.02.2026.
//

#ifndef SDL_TEST_GAMEMANAGER_H
#define SDL_TEST_GAMEMANAGER_H
#include <memory>
#include <vector>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_render.h"


struct BoxCollider;
class GameObject;

class GameManager {
public:
    GameManager();

    ~GameManager();

    bool init(int window_width, int window_height);

    // void handleEvent(SDL_Event *event);
    void handleSystemEvent(SDL_Event *event);

    void handleGameEvent(SDL_Event *event);

    void update();

    void updateTick(Uint64 tick);

    void render();
    void addObject(std::unique_ptr<GameObject> obj);
    SDL_Renderer *getRenderer();

    /**
     * destroy all sdl objects
     */
    void shutdown();

    // Методы для работы с объектами
    // GameObject *addPlatform(const sprites::SpriteInfo *tile, float x, float y, const std::vector<BoxCollider> &boxes);

    GameObject *spawnPlayer(float x, float y);

private:
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    int width = 0;
    int height = 0;

    std::unique_ptr<GameObject> player;
    std::vector<std::unique_ptr<GameObject> > platforms;

    // TexturePtr bgGradient;
    // TexturePtr mainTex;
    float worldOffsetX = 0.0f;
    Uint64 lastTick = 0;
    float accumulator = 0.0f;

    bool moveLeft = false;
    bool moveRight = false;
    bool jumpPressed = false;
};


#endif //SDL_TEST_GAMEMANAGER_H
