//
// Created by zhenya on 13.02.2026.
//

#ifndef SDL_TEST_COLLIDER_H
#define SDL_TEST_COLLIDER_H
#include <vector>

#include "GameObject.h"

struct BoxCollider {
    float ox, oy;
    float w, h;
};

class Collider {
    public:
    std::vector<BoxCollider> boxes;
    bool solid = false;
    bool collisionDetection(const GameObject* selfObj, const GameObject* otherObj) const;
};


#endif //SDL_TEST_COLLIDER_H