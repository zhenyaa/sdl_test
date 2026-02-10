//
// Created by zhenya on 10.02.2026.
//

#ifndef SDL_TEST_SPRITES_H
#define SDL_TEST_SPRITES_H
#include <SDL3/SDL_rect.h>

namespace Sprites {

    // Player
    inline constexpr SDL_FRect PlayerIdle {
        0, 16, 15, 15
    };

    inline constexpr SDL_FRect PlayerRun1 {
        16, 16, 15, 15
    };

    inline constexpr SDL_FRect PlayerRun2 {
        32, 16, 15, 15
    };


    // Blocks
    inline constexpr SDL_FRect Ground {
        0, 0, 15, 15
    };

    inline constexpr SDL_FRect Stone {
        16, 0, 15, 15
    };

}
#endif //SDL_TEST_SPRITES_H