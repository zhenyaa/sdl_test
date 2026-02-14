//
// Created by zhenya on 13.02.2026.
//

#ifndef SDL_TEST_COLLIDER_H
#define SDL_TEST_COLLIDER_H
#include <vector>
class GameObject;

struct BoxCollider {
    float ox, oy;
    float w, h;
};

class Collider {
    public:
    bool solid = false;
    std::vector<BoxCollider> boxes;
    static bool collisionDetection(const GameObject* obj, const GameObject* otherObj);
};


#endif //SDL_TEST_COLLIDER_H