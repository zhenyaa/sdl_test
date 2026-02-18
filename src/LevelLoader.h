//
// Created by zhenya on 18.02.2026.
//

#ifndef SDL_TEST_LEVELLOADER_H
#define SDL_TEST_LEVELLOADER_H
#include <string>


class GameManager;

class LevelLoader {
public:
    static void spawn(
       GameManager& gm,
       const std::string& prefab,
       float x,
       float y,
       float scale = 1.0f
   );
};


#endif //SDL_TEST_LEVELLOADER_H