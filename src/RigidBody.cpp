//
// Created by zhenya on 10.02.2026.
//

#include "RigidBody.h"

#include <iostream>

#include "GameObject.h"
void RigidBody::update(float dt) {
    if (!obj) return;
    std::cout << "rb velocity " << velY << std::endl;
    const int groundLevel = 950;
    velY += gravity * dt;
    obj->y += static_cast<int>(velY * dt);
    if (obj->y + obj->height > groundLevel) {
        obj->y = groundLevel - obj->height;
        velY = 0;
    }
}
